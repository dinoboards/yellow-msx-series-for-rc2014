#include "../ftdi.h"
#include "../vendor_ftdi.h"
#include <extbio/serial.h>
#include <string.h>

uint8_t            assigned_port_number;
uint32_t           assigned_baudrate;
uint8_t            assigned_protocol;
device_config_ftdi _ftdi_config;

uint8_t _serial_get_driver_name(char *const driver_name) {
  strcpy(driver_name, "ftdi");
  return 0;
}

uint8_t _serial_set_baudrate(const int32_t baudrate) {
  assigned_baudrate = baudrate;
  return ftdi_set_baudrate(&_ftdi_config, baudrate);
}

uint8_t _serial_set_flowctrl(const int8_t flowctrl) { return ftdi_set_flowctrl(&_ftdi_config, flowctrl); }

uint8_t _serial_set_dtr_rts(const int16_t dtr_rts_flags) { return ftdi_set_dtr_rts(&_ftdi_config, dtr_rts_flags); }

uint8_t _serial_set_protocol(const uint16_t protocol) {
  assigned_protocol = protocol;

  return ftdi_set_line_property2(&_ftdi_config, protocol);
}

uint8_t _serial_read(uint8_t *const buf, uint16_t *size) { return ftdi_read_data(&_ftdi_config, buf, size); }

uint8_t _serial_demand_read(uint8_t *const buf, uint16_t *size, const uint16_t timeout_ms) {
  return ftdi_demand_read_data(&_ftdi_config, buf, size, timeout_ms);
}

uint8_t _serial_write(const uint8_t *const buf, const uint8_t size) { return ftdi_write_data(&_ftdi_config, buf, size); }

uint8_t _serial_purge_buffers(void) {
  uint8_t result = ftdi_purge_tx_buffer(&_ftdi_config);
  if (result)
    return result;

  return ftdi_purge_rx_buffer(&_ftdi_config);
}
