#ifndef __USB_DEV_INFO_UFI
#define __USB_DEV_INFO_UFI

#include "ch376.h"
#include "nextor.h"
#include <stdlib.h>

extern uint8_t
usb_dev_info_ufi(storage_device_config *const dev, const dev_info_request request_info, uint8_t *const buffer);

#endif
