#include "work-area.h"

_usb_state *get_usb_work_area() {
  work_area *const p = get_work_area();
  return &p->ch376;
}
