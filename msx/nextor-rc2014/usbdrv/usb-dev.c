#include "usb-dev.h"

// device_config *find_usb_driver(const usb_device_type type, const uint8_t * counter) {

//   const device_config* const p = get_usb_work_area()->storage_device;

// }

device_config *get_usb_driver(const uint8_t device_index) __z88dk_fastcall {
  const device_config *const p = get_usb_work_area()->storage_device;

  uint8_t counter = 1;
  int8_t  i;
  for (i = MAX_NUMBER_OF_STORAGE_DEVICES - 1; i >= 0; i--) {
    if (p[i].type == USB_IS_FLOPPY) {
      if (counter == device_index)
        return &p[i];
      counter++;
    }
  }

  for (i = MAX_NUMBER_OF_STORAGE_DEVICES - 1; i >= 0; i--) {
    if (p[i].type == USB_IS_MASS_STORAGE) {
      if (counter == device_index)
        return &p[i];
      counter++;
    }
  }

  for (i = MAX_NUMBER_OF_STORAGE_DEVICES - 1; i >= 0; i--) {
    if (p[i].type == USB_NOT_SUPPORTED) {
      if (counter == device_index)
        return &p[i];
      counter++;
    }
  }

  return NULL; // should not be able to get here
}
