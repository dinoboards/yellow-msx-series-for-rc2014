#ifndef __READ_AND_WRITE_SECTORS_H
#define __READ_AND_WRITE_SECTORS_H

#include <ch376.h>
#include <dev_transfers.h>
#include <stdint.h>

extern usb_error
read_sector(device_config *const storage_device, const uint16_t sector_number, const uint8_t sector_count, uint8_t *buffer)
    __sdcccall(1);
extern usb_error
write_sector(device_config *const storage_device, const uint16_t sector_number, const uint8_t sector_count, uint8_t *buffer)
    __sdcccall(1);

#endif
