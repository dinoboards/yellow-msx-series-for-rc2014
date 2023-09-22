#include "usb_state.h"

_usb_state x;

_usb_state *get_usb_work_area() { return &x; }
