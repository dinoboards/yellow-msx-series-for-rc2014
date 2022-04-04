#ifndef __USB_ENUMERATE_FLOPPY
#define __USB_ENUMERATE_FLOPPY

#include "usb.h"
#include "work-area.h"

extern void parse_endpoint_floppy(_usb_state *const work_area, const endpoint_descriptor const *pEndpoint);

extern void configure_usb_floppy(_usb_state *const work_area);

#endif
