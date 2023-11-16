#include "../sio2.h"
#include <string.h>

uint8_t assigned_port_number;

uint8_t _serial_get_driver_name(char *const driver_name) {
  strcpy(driver_name, "sio/2");
  return 0;
}

uint8_t _serial_set_baudrate(const int32_t baudrate) {
  baudrate;
  return 1;
}

uint8_t _serial_set_flowctrl(const int8_t flowctrl) {
  flowctrl;
  return 1;
}

uint8_t _serial_set_dtr_rts(const int16_t dtr_rts_flags) {
  dtr_rts_flags;
  return 1;
}

uint8_t _serial_set_protocol(const uint16_t protocol) {
  protocol;
  return 1;
}

uint8_t _serial_read(uint8_t *const buf, uint16_t *size) {
  buf;
  size;
  return 1;
}

uint8_t _serial_demand_read(uint8_t *const buf, uint16_t *size, const uint16_t timeout_ms) {
  buf;
  size;
  timeout_ms;
  return 1;
}

uint8_t _serial_write(const uint8_t *const buf, const uint8_t size) {
  buf;
  size;
  return 1;
}

uint8_t _serial_purge_buffers(void) { return 1; }
