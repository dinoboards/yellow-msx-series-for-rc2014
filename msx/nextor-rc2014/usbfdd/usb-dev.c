#include "usb-dev.h"

storage_device_config *get_usb_driver(const uint8_t device_index) __z88dk_fastcall {
  return &(get_usb_work_area()->storage_device[device_index - 1]);
}
