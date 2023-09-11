#include "work-area.h"

work_area x;

_usb_state *get_usb_work_area() {
  work_area *const p = &x;
  return &p->ch376;
}

work_area *get_work_area() { return &x; }
