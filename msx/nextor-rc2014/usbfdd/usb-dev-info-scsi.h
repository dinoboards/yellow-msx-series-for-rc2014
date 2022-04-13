#ifndef __USB_DEV_INFO_SCSI
#define __USB_DEV_INFO_SCSI

#include "ch376.h"
#include "nextor.h"

uint8_t usb_dev_info_scsi(storage_device_config *const dev, const dev_info_request request_info, uint8_t *const buffer);

#endif
