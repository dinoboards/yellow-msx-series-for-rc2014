#ifndef __USB_DEV_INFO_UFI
#define __USB_DEV_INFO_UFI

#include "nextor.h"
#include "protocol.h"
#include <ch376.h>
#include <stdlib.h>

extern uint8_t usb_dev_info_ufi(device_config *const dev, const dev_info_request request_info, uint8_t *const buffer);

#endif
