#ifndef __ENUMERATE_TRACE
#define __ENUMERATE_TRACE

#if defined(ENUMERATE_TRACE_ENABLED)

#include "class_hub.h"
#include "class_ufi.h"
#include "usb_state.h"
#include <stdbool.h>

extern bool logEnabled;

#define trace_printf printf

#define x_printf(...)                                                                                                              \
  if (logEnabled) {                                                                                                                \
    trace_printf(__VA_ARGS__);                                                                                                     \
  }

extern void logInterface(const interface_descriptor *const p);

extern void logConfig(const config_descriptor *const p);

extern void logDevice(const device_descriptor *const p);

extern void logEndPointDescription(const endpoint_descriptor *const p);

extern void logWorkArea(const _usb_state *const p);

extern void logInquiryResponse(const ufi_inquiry_response *const response);

extern void logSetupPacket(const setup_packet *const cmd_packet);

// extern void logNextorLunInfo(const nextor_lun_info *const info);

extern void logHubDescription(const hub_descriptor *const hub_descriptor);

extern void logHubPortStatus(const hub_port_status *const port_status);

#else

#define trace_printf(...)
#define logInterface(...)
#define logConfig(...)
#define logDevice(...)
#define logEndPointDescription(...)
#define logWorkArea(...)
#define logInquiryResponse(...)
#define logSetupPacket(...)
// #define logNextorLunInfo(...)
#define logHubDescription(...)
#define logHubPortStatus(...)

#endif

#endif
