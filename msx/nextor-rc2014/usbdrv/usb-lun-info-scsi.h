#ifndef __USB_LUN_INFO_SCSI
#define __USB_LUN_INFO_SCSI

#include "ch376.h"
#include "nextor.h"
#include <dev_transfers.h>
#include <stdlib.h>

extern uint8_t usb_lun_info_scsi(device_config *const dev, const uint8_t lun, nextor_lun_info *const info);

#endif
