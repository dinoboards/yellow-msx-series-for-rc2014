#include "../ftdi.h"
#include "../vendor_ftdi.h"
#include <extbio/serial.h>
#include <string.h>

uint8_t            assigned_port_number;
uint32_t           assigned_baudrate;
uint8_t            assigned_protocol;
device_config_ftdi _ftdi_config;

uint8_t serial_get_driver_name(const uint8_t port_number, char *const driver_name) {
  if (port_number != assigned_port_number)
    return 1;

  strcpy(driver_name, "ftdi");
  return 0;
}

uint8_t serial_set_baudrate(const uint8_t port_number, const int32_t baudrate) {
  if (port_number != assigned_port_number)
    return 1;

  assigned_baudrate = baudrate;
  return ftdi_set_baudrate(&_ftdi_config, baudrate);
}

uint8_t serial_set_flowctrl(const uint8_t port_number, const int8_t flowctrl) {
  if (port_number != assigned_port_number)
    return 1;

  return ftdi_set_flowctrl(&_ftdi_config, flowctrl);
}

uint8_t serial_set_dtr_rts(const uint8_t port_number, const int16_t dtr_rts_flags) {
  if (port_number != assigned_port_number)
    return 1;

  return ftdi_set_dtr_rts(&_ftdi_config, dtr_rts_flags);
}

uint8_t serial_set_protocol(const uint8_t port_number, const uint16_t protocol) {
  if (port_number != assigned_port_number)
    return 1;

  assigned_protocol = protocol;

  return ftdi_set_line_property2(&_ftdi_config, protocol);
}

uint8_t serial_read(const uint8_t port_number, uint8_t *const buf, uint16_t *size) {
  if (port_number != assigned_port_number)
    return 1;

  return ftdi_read_data(&_ftdi_config, buf, size);
}

uint8_t serial_demand_read(const uint8_t port_number, uint8_t *const buf, uint16_t *size, const uint16_t timeout_ms) {
  if (port_number != assigned_port_number)
    return 1;

  return ftdi_demand_read_data(&_ftdi_config, buf, size, timeout_ms);
}

uint8_t serial_write(const uint8_t port_number, const uint8_t *const buf, const uint8_t size) {
  if (port_number != assigned_port_number)
    return 1;

  return ftdi_write_data(&_ftdi_config, buf, size);
}

uint8_t serial_purge_buffers(const uint8_t port_number) __z88dk_fastcall {
  if (port_number != assigned_port_number)
    return 1;

  uint8_t result = ftdi_purge_tx_buffer(&_ftdi_config);
  if (result)
    return result;

  return ftdi_purge_rx_buffer(&_ftdi_config);
}
