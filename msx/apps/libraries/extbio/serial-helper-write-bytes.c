#include "msxdos.h"
#include "serial-helpers.h"
#include <stdio.h>
#include <system_vars.h>
#include <z80.h>

extern uint8_t port_number;

extern char serial_write_temp_buffer[256];

uint8_t serial_write_bytes_h(const uint8_t *str, const uint8_t len) __sdcccall(1) {
  memcpy(serial_write_temp_buffer, str, len);
  return serial_write(port_number, serial_write_temp_buffer, len);
}
