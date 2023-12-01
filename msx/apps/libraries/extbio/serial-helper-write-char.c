#include "msxdos.h"
#include "serial-helpers.h"
#include <stdio.h>
#include <system_vars.h>
#include <z80.h>

extern uint8_t port_number;

uint8_t serial_write_char(const uint8_t c) { return serial_write(port_number, &c, 1); }
