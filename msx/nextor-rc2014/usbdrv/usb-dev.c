#include "usb-dev.h"

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

/**
 * @brief Retrieve hardware probe status of the USB Module
 *
 * @return uint16_t low byte is the number of USB devices present, high byte is 1 if CH376 is present
 */
uint16_t extbio_rc2014_usb_get_present() {
  uint8_t l = 0;

  work_area *const p = get_work_area();

  if (!(p->present & PRES_CH376))
    return 0;

  return (uint16_t)p->ch376.count_of_detected_usb_devices | 0x0100;
}
