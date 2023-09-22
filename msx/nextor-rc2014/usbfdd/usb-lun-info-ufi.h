#ifndef __USB_LUN_INFO_UFI
#define __USB_LUN_INFO_UFI

#include "hw.h"
#include "nextor.h"
#include <ch376.h>
#include <stdlib.h>

extern uint8_t usb_lun_info_ufi(device_config *const dev, const uint8_t lun, nextor_lun_info *const info);

#endif
