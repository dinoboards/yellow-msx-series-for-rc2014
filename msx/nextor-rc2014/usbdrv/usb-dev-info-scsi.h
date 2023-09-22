#ifndef __USB_DEV_INFO_SCSI
#define __USB_DEV_INFO_SCSI

#include "nextor.h"
#include "protocol.h"
#include <ch376.h>

uint8_t usb_dev_info_scsi(device_config *const dev, const dev_info_request request_info, uint8_t *const buffer);

#endif
