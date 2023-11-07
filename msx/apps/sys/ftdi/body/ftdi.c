#include "../ftdi.h"
#include <extbio/serial.h>
#include <string.h>
#include <usb/vendor_ftdi.h>

uint8_t            assigned_port_number;
uint32_t           assigned_baudrate;
uint8_t            assigned_protocol;
device_config_ftdi _ftdi_config;

uint8_t serial_get_driver_name(const uint8_t port_number, char *const driver_name) {
  if (port_number == assigned_port_number) {
    strcpy(driver_name, "ftdi");
    return 0;
  }
  return 1;
}

uint8_t serial_set_baudrate(const uint8_t port_number, const int32_t baudrate) {
  if (port_number == assigned_port_number) {
    assigned_baudrate = baudrate;
    return 0;
  }
  return 1;
}

uint8_t serial_set_protocol(const uint8_t port_number, const uint16_t protocol) {
  if (port_number == assigned_port_number) {
    assigned_protocol = protocol;
    return 0;
  }
  return 1;
}
