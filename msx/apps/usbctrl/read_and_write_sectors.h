#ifndef __READ_AND_WRITE_SECTORS_H
#define __READ_AND_WRITE_SECTORS_H

#include <ch376.h>
#include <dev_transfers.h>

usb_error read_sector(device_config *const storage_device, const uint16_t sector_number, uint8_t *buffer);

#endif
