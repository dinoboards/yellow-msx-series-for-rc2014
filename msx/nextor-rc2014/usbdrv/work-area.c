#include "work-area.h"

_usb_state *get_usb_work_area() {
  work_area *const p = get_work_area();
  return &p->ch376;
}

uint8_t get_number_of_usb_drives() {
  const work_area *const p = get_work_area();
  uint8_t                c = 0;

  const device_config *const last = &p->ch376.storage_device[MAX_NUMBER_OF_STORAGE_DEVICES - 1];
  for (device_config *dev = p->ch376.storage_device; dev <= last; dev++) {
    if (dev->address != 0)
      c++;
  }
  return c;
}
