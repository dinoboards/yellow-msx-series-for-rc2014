#include "work-area.h"

_usb_state *get_usb_work_area() {
  work_area *const p = get_work_area();
  return &p->ch376;
}

uint8_t get_number_of_usb_drives() {
  work_area *const p = get_work_area();
  return p->ch376.next_storage_device_index + 1;
}
