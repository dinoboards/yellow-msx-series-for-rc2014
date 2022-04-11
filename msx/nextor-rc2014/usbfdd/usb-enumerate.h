#ifndef __USB_ENUMERATE
#define __USB_ENUMERATE

#include "usb.h"
#include "work-area.h"

extern usb_error read_all_configs(uint8_t const *next_storage_device_index);
extern usb_error enumerate_all_devices();

#endif
