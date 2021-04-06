//
//  nvme.h
//  smartTBW
//
//  Created by James Bailey on 4/5/21.
//

#ifndef nvme_h
#define nvme_h

#include <stdio.h>
#include <inttypes.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/storage/nvme/NVMeSMARTLibExternal.h>

static struct device {
  io_object_t ioob;
  IOCFPlugInInterface **plugin;
  IONVMeSMARTInterface **smartIfNVMe;
} device;

io_object_t detect_nvme_device(void);
bool open_nvme_device(io_object_t disk);
bool close_nvme_device(void);
void nvme_read_data(NVMeSMARTData *data);

#endif /* nvme_h */
