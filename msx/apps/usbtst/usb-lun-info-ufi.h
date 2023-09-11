#ifndef __USB_LUN_INFO_UFI
#define __USB_LUN_INFO_UFI

#include "ch376.h"
#include "nextor.h"
#include <stdlib.h>

extern uint8_t usb_lun_info_ufi(storage_device_config *const dev, const uint8_t lun, nextor_lun_info *const info);

#endif
