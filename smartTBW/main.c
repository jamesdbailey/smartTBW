//
//  main.c
//  smartTBW
//
//  Created by James Bailey on 4/5/21.
//

#include <stdio.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/storage/nvme/NVMeSMARTLibExternal.h>

static struct {
  io_object_t ioob;
  IOCFPlugInInterface **plugin;
  IONVMeSMARTInterface **smartIfNVMe;
} device;

bool is_smart_capable (io_object_t dev) {
    CFTypeRef smartCapableKey = NULL;

    smartCapableKey = IORegistryEntryCreateCFProperty
      (dev, CFSTR (kIOPropertyNVMeSMARTCapableKey),
       kCFAllocatorDefault, 0);
    if (smartCapableKey)
    {
        CFRelease (smartCapableKey);
        return true;
    }

    return false;
}

io_object_t detect_nvme_device() {
    CFMutableDictionaryRef matcher;
    const char *devname = "disk0";
    io_object_t disk;
    matcher = IOBSDNameMatching (kIOMasterPortDefault, 0, devname);
    disk = IOServiceGetMatchingService (kIOMasterPortDefault, matcher);

    io_registry_entry_t tmpdisk=disk;

    while (! is_smart_capable (tmpdisk))
    {
        IOReturn err;
        io_object_t prevdisk = tmpdisk;

        // Find this device's parent and try again.
        err = IORegistryEntryGetParentEntry (tmpdisk, kIOServicePlane, &tmpdisk);
        if (err != kIOReturnSuccess || ! tmpdisk)
        {
          IOObjectRelease (prevdisk);
          break;
        }
    }
    
    if (tmpdisk)
    {
        return tmpdisk;
    }

    return 0;
}

bool open_nvme_device(io_object_t disk)
{
    SInt32 dummy;
    CFUUIDRef pluginType = NULL;
    CFUUIDRef smartInterfaceId = NULL;
    void ** SMARTptr = NULL;

    device.ioob = MACH_PORT_NULL;
    device.plugin = NULL;
    device.smartIfNVMe = NULL;
    SMARTptr = (void **)&device.smartIfNVMe;

    pluginType = kIONVMeSMARTUserClientTypeID;
    smartInterfaceId = kIONVMeSMARTInterfaceID;
    
    kern_return_t res = IOCreatePlugInInterfaceForService (disk, pluginType, kIOCFPlugInInterfaceID, &device.plugin, &dummy);
    
    if (res == kIOReturnSuccess)
    {
        CFUUIDBytes bytes = CFUUIDGetUUIDBytes (smartInterfaceId);
        (*device.plugin)->QueryInterface(device.plugin, bytes, SMARTptr);
        device.ioob = disk;
        return true;
    }

    return false;
}

bool close_nvme_device()
{
    if (device.ioob)
    {
        if (device.smartIfNVMe)
          (*device.smartIfNVMe)->Release (device.smartIfNVMe);
        if (device.plugin)
          IODestroyPlugInInterface (device.plugin);
        IOObjectRelease (device.ioob);
        device.ioob = MACH_PORT_NULL;
    }
    
    return true;
}

int main(int argc, const char * argv[]) {
    io_object_t disk = detect_nvme_device();
    if (disk)
        printf("found one nvme\n");
    else
        printf("didn't find nvme\n");
    
    if (disk)
    {
        if (open_nvme_device(disk))
        {
            NVMeSMARTData data;
            (*device.smartIfNVMe)->SMARTReadData(device.plugin, &data);
            close_nvme_device();
        }
    }
    
    return 0;
}
