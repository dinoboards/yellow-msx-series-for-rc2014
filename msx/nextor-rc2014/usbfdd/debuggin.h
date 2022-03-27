#ifndef __DEBUGGIN
#define __DEBUGGIN

#include "ufi.h"
#include "usb.h"
#include "work-area.h"

extern void logInterface(const interface_descriptor *const p);

extern void logConfig(const config_descriptor *const p);

extern void logDevice(const device_descriptor *const p);

extern void logEndPointDescription(const endpoint_descriptor *const p);

extern void logWorkArea(const _usb_state *const p);

extern void logInquiryResponse(const ufi_inquiry_response *const response);

extern void logSetupPacket(const setup_packet *const cmd_packet);

#endif
