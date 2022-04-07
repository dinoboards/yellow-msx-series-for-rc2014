#ifndef __USB_ENUMERATE_HUB
#define __USB_ENUMERATE_HUB

#include "usb.h"
#include "work-area.h"

extern void      parse_endpoint_hub(_usb_state *const work_area, const endpoint_descriptor const *pEndpoint);
extern usb_error configure_usb_hub(_usb_state *const work_area);

#endif
