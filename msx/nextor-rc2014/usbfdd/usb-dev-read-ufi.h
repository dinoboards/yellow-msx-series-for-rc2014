#ifndef __USB_DEV_READ_UFI
#define __USB_DEV_READ_UFI

#include "work-area.h"
#include <stdlib.h>

uint8_t usb_dev_read_ufi(storage_device_config *const dev,
                         const uint8_t                lun,
                         uint8_t                      number_sectors_to_read,
                         uint32_t                     sector_number,
                         uint8_t *                    buffer,
                         uint8_t *const               number_of_sectors_read);

#endif
