#ifndef __CLASS_PRINTER
#define __CLASS_PRINTER

#include "ch376.h"
#include "protocol.h"

usb_error prt_get_port_status(const device_config_printer *const dev, uint8_t *buffer);

usb_error prt_get_device_id(const device_config_printer *const dev, uint8_t *buffer);

usb_error prt_soft_reset(const device_config_printer *const dev);

usb_error prt_send_text(device_config_printer *dev, const char *text);

#endif
