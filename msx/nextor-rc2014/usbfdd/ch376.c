#include "ch376.h"
#include <delay.h>
#include <stdbool.h>

#include "print.h"

const void setCommand(const uint8_t command) __z88dk_fastcall {
  CH376_COMMAND_PORT = command;
  for (uint8_t i = 255; i != 0; i--)
    ;
}

uint8_t ch_wait_int_and_get_result() {
  uint8_t counter = 255;
  while ((CH376_DATA_PORT & 0x80) && --counter != 0)
    ;

  setCommand(CH_CMD_GET_STATUS);
  return CH376_DATA_PORT;
}

uint8_t *ch_read_data(uint8_t *buffer, uint16_t buffer_size, int8_t *const amount_received) {
  setCommand(CH_CMD_RD_USB_DATA0);
  uint8_t count = CH376_DATA_PORT;
  *amount_received = count;
  uint8_t extra = 0;

  if (count > buffer_size) {
    extra = count - buffer_size;

    yprintf(0, "Y (%d,", count);
    xprintf("%d,", buffer_size);
    xprintf("%d)", extra);

    count = buffer_size;
  }

  while (count-- != 0)
    *buffer++ = CH376_DATA_PORT;

  while (extra-- != 0) // do we need to flush buffer?
    CH376_DATA_PORT;

  return buffer;
}

void ch376_reset() {
  delay(30);
  setCommand(CH_CMD_RESET_ALL);
  delay(30);
}

inline uint8_t ch376_test() {
  setCommand(CH_CMD_CHECK_EXIST);
  CH376_DATA_PORT = (uint8_t)~0x34;
  if (CH376_DATA_PORT != 0x34)
    return false;

  setCommand(CH_CMD_CHECK_EXIST);
  CH376_DATA_PORT = (uint8_t)~0x89;
  return CH376_DATA_PORT == 0x89;
}

uint8_t ch376_probe() {
  for (int8_t i = 8; i > 0; i--) {
    if (ch376_test())
      return true;

    delay(5);
  }

  return false;
}

uint8_t ch376_set_usb_mode(const uint8_t mode) __z88dk_fastcall {
  uint8_t result;
  setCommand(CH_CMD_SET_USB_MODE);
  CH376_DATA_PORT = mode;

  result = ch_wait_int_and_get_result();
  if (result != CH_USB_INT_SUCCESS)
    return result;

  return CH_USB_INT_SUCCESS;
}

uint8_t ch376_get_firmware_version() {
  setCommand(CH_CMD_GET_IC_VER);
  return CH376_DATA_PORT & 0x1f;
}

const uint8_t *ch_write_data(const uint8_t *buffer, uint8_t length) {
  setCommand(CH_CMD_WR_HOST_DATA);
  CH376_DATA_PORT = length;

  while (length-- != 0) {
    CH376_DATA_PORT = *buffer++;
  }

  return buffer;
}

void ch_issue_token(const uint8_t endpoint, const ch376_pid pid, const uint8_t toggle_bits) {
  setCommand(CH_CMD_ISSUE_TKN_X);
  CH376_DATA_PORT = toggle_bits;
  CH376_DATA_PORT = endpoint << 4 | pid;
}

uint8_t ch_data_in_transfer(uint8_t *buffer, int16_t data_length, const uint8_t max_packet_size, const uint8_t endpoint, uint16_t *const amount_received, uint8_t *const toggle) {
  uint8_t count;
  uint8_t result;
  do {
    ch_issue_token(endpoint, CH_PID_IN, *toggle ? 0x80 : 0x00);
    *toggle = ~*toggle;

    if ((result = ch_wait_int_and_get_result()) != CH_USB_INT_SUCCESS)
      return result;

    buffer = ch_read_data(buffer, data_length, &count);
    data_length -= count;
    *amount_received += count;
  } while (data_length > 0 && count <= max_packet_size);

  return CH_USB_INT_SUCCESS;
}

uint8_t ch_data_out_transfer(const uint8_t *buffer, int16_t buffer_length, const uint8_t max_packet_size, const uint8_t endpoint, uint8_t *const toggle) {
  uint8_t result;

  while (buffer_length > 0) {
    const uint8_t size = max_packet_size < buffer_length ? max_packet_size : buffer_length;
    buffer = ch_write_data(buffer, size);
    buffer_length -= size;
    ch_issue_token(endpoint, CH_PID_OUT, *toggle ? 0x40 : 0x00);
    if ((result = ch_wait_int_and_get_result()) != CH_USB_INT_SUCCESS)
      return result;

    *toggle = ~*toggle;
  }

  return CH_USB_INT_SUCCESS;
}
