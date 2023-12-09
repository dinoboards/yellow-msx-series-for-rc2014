#include "../tty.h"
#include "tty.h"
#include <msxbios.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

extern uint8_t serial_write_di(const uint8_t port_number, const uint8_t *const buf, const uint8_t size);

static uint8_t     tracking_escape_sequence;
static uint8_t     tracking_x_sequence;
static uint8_t     tracking_y_sequence;
static const char *sequence = NULL;

/**
 * @brief convert the msx char code to an ansi char code
 * and forward to serial connection
 *
 * @param c
 */
void chput_to_serial(const unsigned char c) __z88dk_fastcall {

  if (tracking_escape_sequence) {
    tracking_escape_sequence = false;

    if (c == 'x') {
      tracking_x_sequence = true;
      return;
    }

    if (c == 'y') {
      tracking_x_sequence = true;
      return;
    }

    return;
  }

  if (tracking_y_sequence) {
    tracking_y_sequence = false;

    // if (c == '4') { //cursor overtype (full size) mode
    //   sequence = "\x1B[2 q";
    //   goto send_sequence;
    // }

    if (c == '5') { // show the cursor
      sequence = "\x1B[?25h";
      goto send_sequence;
    }

    return;
  }

  if (tracking_x_sequence) {
    tracking_x_sequence = false;

    // if (c == '4') { //cursor insert mode
    //   sequence = "\x1B[4 q";
    //   goto send_sequence;
    // }

    if (c == '5') { // hide the cursor
      sequence = "\x1B[?25l";
      goto send_sequence;
    }

    return;
  }

  switch (c) {
  case CHAR_MOVE_LEFT:
    sequence = "\x1b[D";
    goto send_sequence;

  case CHAR_MOVE_RIGHT:
    sequence = "\x1b[C";
    goto send_sequence;

  case CHAR_ESCAPE:
    tracking_escape_sequence = true;
    return;
  }

  serial_write_di(activate_stdout_port_number, &c, 1);
  return;

send_sequence:
  serial_write_di(activate_stdout_port_number, sequence, strlen(sequence));
  return;
}
