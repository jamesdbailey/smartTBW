//
//  main.c
//  smartTBW
//
//  Created by James Bailey on 4/5/21.
//

#include <stdio.h>
#include <inttypes.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/storage/nvme/NVMeSMARTLibExternal.h>
#include "utility.h"
#include "nvme.h"

void print_data_128_cap(uint64_t *data, char *prefix)
{
    char cap[64];
    char num[64];
    uint64_t low = data[0];
    uint64_t high = data[1];
    format128(num, sizeof(num), low, high);
    uint128_t u128 = lowhigh_to_uint128(low, high);
    u128 = u128 * 1000 * 512;
    format_capacity(cap, sizeof(cap), u128);
    printf("%s = %s [%s]\n", prefix, num, cap);
}

void print_data_128(uint64_t *data, char *prefix)
{
    char num[64];
    uint64_t low = data[0];
    uint64_t high = data[1];
    format128(num, sizeof(num), low, high);
    printf("%s = %s\n", prefix, num);
}

int main(int argc, const char * argv[]) {
    io_object_t disk = detect_nvme_device();
    if (disk)
        printf("found one NVMe at disk0\n");
    else
        printf("didn't find NVMe\n");
    
    if (disk)
    {
        if (open_nvme_device(disk))
        {
            NVMeSMARTData data;
            nvme_read_data(&data);
            close_nvme_device();
            print_data_128_cap(data.DATA_UNITS_READ, "Data Units Read");
            print_data_128_cap(data.DATA_UNITS_WRITTEN, "Data Units Written");
            print_data_128(data.HOST_READ_COMMANDS, "Host Read Commands");
            print_data_128(data.HOST_WRITE_COMMANDS, "Host Write Commands");
        }
    }

    return 0;
}
