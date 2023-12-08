#include "../tty.h"
#include <extbio/serial.h>
#include <msxbios/hooks.h>
#include <stdint.h>
#include <string.h>
#include <system_vars.h>

extern uint8_t serial_read_di(const uint8_t port_number, uint8_t *const buf, uint16_t *const size);
extern uint8_t serial_write_di(const uint8_t port_number, const uint8_t *const buf, const uint8_t size);

uint8_t activate_stdin_port_number  = 0;
uint8_t activate_stdout_port_number = 0;
uint8_t activate_keyboard           = 1;
uint8_t activate_crt                = 1;
uint8_t last_screen_mode            = 255;

uint8_t tty_update(const tty_settings_t *const settings) __sdcccall(1) {
  activate_stdin_port_number = settings->inputs.com;
  activate_keyboard          = settings->inputs.console;

  activate_stdout_port_number = settings->outputs.com;
  activate_crt                = settings->outputs.console;

  if (activate_keyboard)
    SCNCNT = 1;

  if (!activate_crt) {
    if (last_screen_mode != 255) {
      SCRMOD           = last_screen_mode;
      last_screen_mode = 255;
    }
  }

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

void _timi(void) {
  if (++limit % 8 != 0)
    goto finish;

  if (activate_crt == 0 && SCRMOD != 255) {
    last_screen_mode = SCRMOD;
    SCRMOD           = 255;
  }

  if (!activate_keyboard)
    SCNCNT = 127;

  if (activate_stdin_port_number == 0)
    goto finish;

  uint16_t      size = 32;
  unsigned char buffer[32];

  uint8_t result = serial_read_di(activate_stdin_port_number, buffer, &size);
  if (size == 0 || result)
    goto finish;

  unsigned char *p = buffer;
  for (uint8_t i = size; i > 0; i--)
    key_put_into_buf(*p++);

finish:
}

void chput_to_serial(const unsigned char c) __z88dk_fastcall { serial_write_di(activate_stdout_port_number, &c, 1); }
