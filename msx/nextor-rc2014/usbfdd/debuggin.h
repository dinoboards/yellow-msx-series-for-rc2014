#ifndef __DEBUGGIN
#define __DEBUGGIN

#include "usb.h"

extern void logInterface(const interface_descriptor *const p);

extern void logConfig(const config_descriptor *const p);

extern void logDevice(const device_descriptor *const p);

extern void logEndPointDescription(const endpoint_descriptor *const p);

// extern void logUsbDevice(const _usb_device_info *const info);

#endif
