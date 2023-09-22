#ifndef __USB_DEV_STATUS_SCSI
#define __USB_DEV_STATUS_SCSI

#include "ch376.h"
#include "protocol.h"
#include <stdlib.h>

uint8_t usb_dev_status_scsi(device_config *const storage_device, const uint8_t lun);

#endif
