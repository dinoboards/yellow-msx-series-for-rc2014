#ifndef __USB_DEV
#define __USB_DEV

#include "ch376.h"
#include "work-area.h"
#include <stdbool.h>

extern device_config *get_usb_device_config(const uint8_t device_index) __z88dk_fastcall;

#endif
