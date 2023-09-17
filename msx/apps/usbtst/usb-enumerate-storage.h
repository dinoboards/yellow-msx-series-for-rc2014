#ifndef __USB_ENUMERATE_FLOPPY
#define __USB_ENUMERATE_FLOPPY

#include "hw.h"
#include "work-area.h"

extern void parse_endpoint_storage(device_config *const storage_dev, const endpoint_descriptor const *pEndpoint);

#endif
