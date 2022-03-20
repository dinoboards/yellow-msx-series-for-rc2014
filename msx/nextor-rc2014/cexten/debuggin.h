#ifndef __DEBUGGIN
#define __DEBUGGIN

#include <stdlib.h>
#include "print.h"
#include "work-area.h"

extern void logConfig(const config_descriptor* const p);

extern void logDevice(const device_descriptor* const p);

extern void logUsbDevice(const _usb_device_info* const info);

#endif
