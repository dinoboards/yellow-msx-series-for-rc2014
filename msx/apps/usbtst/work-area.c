#include "usb_state.h"

_usb_state x;

_usb_state *get_usb_work_area(void) { return &x; }

_usb_state y;

_usb_state *get_usb_boot_area(void) { return &y; }
