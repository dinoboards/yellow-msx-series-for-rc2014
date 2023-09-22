#ifndef __USB_ENUMERATE_STORAGE
#define __USB_ENUMERATE_STORAGE

#include "hw.h"

extern void parse_endpoint_storage(device_config *const storage_dev, const endpoint_descriptor const *pEndpoint);

#endif
