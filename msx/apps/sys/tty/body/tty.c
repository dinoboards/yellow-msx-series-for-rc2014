#include "../tty.h"
#include <stdint.h>
#include <string.h>
#include <system_vars.h>

extern uint8_t serial_read_di(const uint8_t port_number, uint8_t *const buf, uint16_t *size);

uint8_t activated_port_number = 0;
uint8_t activate_keyboard     = 1;

uint8_t tty_update(const tty_settings_t *const settings) __sdcccall(1) {
  activated_port_number = settings->inputs.com;
  activate_keyboard     = settings->outputs.console;

  if (activate_keyboard)
    SCNCNT = 1;

  return 0;
}

void key_put_into_buf(const uint8_t code) __z88dk_fastcall {
  char *buffer_put = PUTPNT;

  *buffer_put++ = code;

  const uint8_t x = ((uint16_t)buffer_put & 0xFF);
  if (x == 0x18)
    buffer_put = KEYBUF;

  if (buffer_put != GETPNT)
    PUTPNT = buffer_put;
}

volatile uint8_t limit = 0; // not really volatile, but it seems to make sdcc make slighly more efficient code??

void map_serial_to_keyboard(void) {
  if (++limit % 8 != 0)
    return;

  if (!activate_keyboard)
    SCNCNT = 127;

  uint16_t      size = 32;
  unsigned char buffer[32];

  uint8_t result = serial_read_di(activated_port_number, buffer, &size);
  if (size == 0 || result)
    goto finish;

  unsigned char *p = buffer;
  for (uint8_t i = size; i > 0; i--)
    key_put_into_buf(*p++);

finish:
}
