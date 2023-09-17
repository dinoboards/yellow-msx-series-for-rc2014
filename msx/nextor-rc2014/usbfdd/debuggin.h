#ifndef __DEBUGGIN
#define __DEBUGGIN

#include "class-hub.h"
#include "class-ufi.h"
#include "nextor.h"
#include "work-area.h"

extern void logInterface(const interface_descriptor *const p);

extern void logConfig(const config_descriptor *const p);

extern void logDevice(const device_descriptor *const p);

extern void logEndPointDescription(const endpoint_descriptor *const p);

extern void logWorkArea(const _usb_state *const p);

extern void logInquiryResponse(const ufi_inquiry_response *const response);

extern void logSetupPacket(const setup_packet *const cmd_packet);

extern void logNextorLunInfo(const nextor_lun_info *const info);

extern void logHubDescription(const hub_descriptor *const hub_descriptor);

extern void logHubPortStatus(const hub_port_status *const port_status);

#endif
