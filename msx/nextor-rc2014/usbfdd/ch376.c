#include "ch376.h"
#include <delay.h>
#include <stdbool.h>

#include "print.h"

const void setCommand(const uint8_t command) __z88dk_fastcall {
  CH376_COMMAND_PORT = command;
  for (uint8_t i = 255; i != 0; i--)
    ;
}

usb_error ch_wait_int_and_get_result() {
  uint8_t counter = 255;
  while ((CH376_COMMAND_PORT & 0x80) && --counter != 0)
    ;

  setCommand(CH_CMD_GET_STATUS);
  uint8_t ch_status = CH376_DATA_PORT;

  if (ch_status >= USB_FILERR_MIN && ch_status <= USB_FILERR_MAX) {
    return ch_status;
  }

  if (ch_status == CH_ST_RET_SUCCESS)
    return USB_ERR_OK;

  if (ch_status == CH_ST_INT_SUCCESS)
    return USB_ERR_OK;

  if (ch_status == CH_ST_INT_DISK_READ)
    return USB_ERR_DISK_READ;

  if (ch_status == CH_ST_INT_DISK_WRITE)
    return USB_ERR_DISK_WRITE;

  if (ch_status == CH_ST_INT_DISCONNECT) {
    ch376_set_usb_mode(5);
    return USB_ERR_NO_DEVICE;
  }

  if (ch_status == CH_ST_INT_BUF_OVER)
    return USB_ERR_DATA_ERROR;

  ch_status &= 0x2F;

  if (ch_status == 0x2A)
    return USB_ERR_NAK;

  if (ch_status == 0x2E)
    return USB_ERR_STALL;

  ch_status &= 23;

  if (ch_status == 0x20)
    return USB_ERR_TIMEOUT;

  return USB_ERR_UNEXPECTED_STATUS_FROM_HOST;
}

uint8_t *ch_read_data(uint8_t *buffer, uint16_t buffer_size, int8_t *const amount_received) {
  setCommand(CH_CMD_RD_USB_DATA0);
  uint8_t count = CH376_DATA_PORT;
  if (amount_received)
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

usb_error ch376_set_usb_mode(const uint8_t mode) __z88dk_fastcall {
  setCommand(CH_CMD_SET_USB_MODE);
  CH376_DATA_PORT = mode;

  return ch_wait_int_and_get_result();
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

usb_error
ch_data_in_transfer(uint8_t *buffer, int16_t buffer_size, endpoint_param *const endpoint, uint16_t *const amount_received) {
  uint8_t   count;
  usb_error result;

  const uint8_t number          = endpoint->number;
  const uint8_t max_packet_size = endpoint->max_packet_size;

  do {
    ch_issue_token(number, CH_PID_IN, endpoint->toggle ? 0x80 : 0x00);
    endpoint->toggle = ~endpoint->toggle;

    if ((result = ch_wait_int_and_get_result()) != USB_ERR_OK)
      return result;

    buffer = ch_read_data(buffer, buffer_size, &count);
    buffer_size -= count;
    *amount_received += count;
  } while (buffer_size > 0 && count <= max_packet_size);

  return USB_ERR_OK;
}

usb_error ch_data_out_transfer(const uint8_t *buffer, int16_t buffer_length, endpoint_param *const endpoint) {
  usb_error     result;
  const uint8_t number          = endpoint->number;
  const uint8_t max_packet_size = endpoint->max_packet_size;

  while (buffer_length > 0) {
    const uint8_t size = max_packet_size < buffer_length ? max_packet_size : buffer_length;
    buffer             = ch_write_data(buffer, size);
    buffer_length -= size;
    ch_issue_token(number, CH_PID_OUT, endpoint->toggle ? 0x40 : 0x00);
    if ((result = ch_wait_int_and_get_result()) != USB_ERR_OK)
      return result;

    endpoint->toggle = ~endpoint->toggle;
  }

  return USB_ERR_OK;
}
