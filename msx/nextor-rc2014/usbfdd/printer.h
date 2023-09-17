#ifndef __PRINTER
#define __PRINTER

#include "ch376.h"
#include "hw.h"

typedef struct __printer_device {
  device_config  config;
  endpoint_param endpoint;
} printer_device_config;

usb_error prt_get_port_status(const device_config *const dev, uint8_t *buffer);

usb_error prt_get_device_id(const device_config *const dev, uint8_t *buffer);

usb_error prt_soft_reset(const device_config *const dev);

usb_error prt_send_text(const printer_device_config *const dev, const char *text);

#endif
