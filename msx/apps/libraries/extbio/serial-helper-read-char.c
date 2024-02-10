#include "msxdos.h"
#include "serial-helpers.h"
#include <stdio.h>
#include <system_vars.h>
#include <z80.h>

extern uint8_t port_number;

uint8_t serial_read_char_h(uint8_t *const c) __sdcccall(1) {
  uint16_t      size   = 1;
  const uint8_t result = serial_demand_read(port_number, c, &size, 1000);
  if (result)
    return result;

  if (size == 0)
    return 255;

  return 0;
}
