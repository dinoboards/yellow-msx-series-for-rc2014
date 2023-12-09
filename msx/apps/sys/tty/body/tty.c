#include "../tty.h"
#include <extbio/serial.h>
#include <msxbios.h>
#include <msxbios/hooks.h>
#include <stdbool.h>
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

uint8_t tracking_escape_sequence  = false;
uint8_t tracking_single_seqeuence = false;

void translate_ansi_to_keyboard_buf(const uint8_t code) __z88dk_fastcall {
  if (tracking_escape_sequence) {
    tracking_escape_sequence = false;

    if (code == '[') {
      tracking_single_seqeuence = true;
      return;
    }

    // if not recognised - just ignore it
    return;
  }

  if (tracking_single_seqeuence) {
    tracking_single_seqeuence = false;

    if (code == 'D') {
      key_put_into_buf(CHAR_MOVE_LEFT);
      return;
    }

    if (code == 'C') {
      key_put_into_buf(CHAR_MOVE_RIGHT);
      return;
    }

    // if not recognised - just ignore it
    return;
  }

  if (code == CHAR_ESCAPE) {
    tracking_escape_sequence  = true;
    tracking_single_seqeuence = false;
    return;
  }

  key_put_into_buf(code);
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
    translate_ansi_to_keyboard_buf(*p++);

finish:
}

void chput_to_serial(const unsigned char c) __z88dk_fastcall {

  if (c == CHAR_MOVE_LEFT) {
    serial_write_di(activate_stdout_port_number, "\x1b[D", 3);
    return;
  }

  if (c == CHAR_MOVE_RIGHT) {
    serial_write_di(activate_stdout_port_number, "\x1b[C", 3);
    return;
  }

  serial_write_di(activate_stdout_port_number, &c, 1);
}
