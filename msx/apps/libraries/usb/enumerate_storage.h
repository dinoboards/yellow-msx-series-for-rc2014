#ifndef __USB_ENUMERATE_STORAGE
#define __USB_ENUMERATE_STORAGE

#include "protocol.h"

extern void parse_endpoint_storage(device_config *const storage_dev, const endpoint_descriptor const *pEndpoint);

#endif
