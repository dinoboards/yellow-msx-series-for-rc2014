#ifndef __USD_DEV_WRITE_UFI
#define __USD_DEV_WRITE_UFI

#include "nextor.h"
#include "protocol.h"
#include <stdlib.h>

uint8_t usb_dev_write_ufi(device_config *const dev,
                          const uint8_t        lun,
                          uint8_t              number_sectors_to_write,
                          uint32_t             sector_number,
                          uint8_t *            buffer,
                          uint8_t *const       number_of_sectors_written);

#endif
