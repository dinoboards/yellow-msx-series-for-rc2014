#ifndef __CLASS_PRINTER
#define __CLASS_PRINTER

#include "ch376.h"
#include "dev_transfers.h"
#include "protocol.h"

#define PRINTER_BUFFER_SIZE 64
typedef struct {
  COMMON_DEVICE_CONFIG
  endpoint_param endpoints[1]; // bulk out
  uint8_t        buffer_length;
  int8_t         buffer_wait;
  uint8_t        buffer[PRINTER_BUFFER_SIZE];
} device_config_printer;

/*

For the USB/Centronics adapters tested against, these operations dont seem to be implemented

usb_error prt_get_port_status(const device_config_printer *const dev, uint8_t *buffer);

usb_error prt_get_device_id(const device_config_printer *const dev, uint8_t *buffer);

usb_error prt_soft_reset(const device_config_printer *const dev);
*/

usb_error prt_send(device_config_printer *dev, const uint8_t *text, const uint8_t length);

#endif
