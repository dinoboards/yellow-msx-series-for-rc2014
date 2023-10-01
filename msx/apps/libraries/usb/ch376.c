#include "ch376.h"
#include <delay.h>
#include <stdbool.h>

void delay_20ms() { delay(1); }

void delay_short() { delay(3); }

void delay_medium() { delay(30); }

void ch_command(const uint8_t command) __z88dk_fastcall {
  uint8_t counter = 255;
  while ((CH376_COMMAND_PORT & PARA_STATE_BUSY) && --counter != 0)
    ;

  // if (counter == 0) {
  // It appears that the Ch376 has become blocked
  // command will fail and timeout will eventually be returned by the ch_xxx_wait_int_and_get_status
  // todo consider a return value to allow callers to respond appropriately
  // Experimentation would indicate that USB_RESET_ALL will still work to reset chip
  // return;
  // }

  CH376_COMMAND_PORT = command;
}

usb_error ch_wait_int_and_get_status(const int16_t timeout) __z88dk_fastcall {
  const int16_t timeout_point = get_future_time(timeout);

  while ((CH376_COMMAND_PORT & PARA_STATE_INTB) && !is_time_past(timeout_point))
    ;

  if ((CH376_COMMAND_PORT & PARA_STATE_INTB)) {
    if (CH376_COMMAND_PORT & PARA_STATE_BUSY)
      return USB_ERR_CH376_BLOCKED;

    return USB_ERR_CH376_TIMEOUT;
  }

  return ch_get_status();
}

usb_error ch_long_wait_int_and_get_status() { return ch_wait_int_and_get_status(from_ms_50hz(5000)); }

usb_error ch_short_wait_int_and_get_status() { return ch_wait_int_and_get_status(from_ms_50hz(100)); }

usb_error ch_very_short_wait_int_and_get_status() { return ch_wait_int_and_get_status(from_ms_50hz(10)); }

usb_error ch_get_status() {
  ch_command(CH_CMD_GET_STATUS);
  uint8_t ch_status = CH376_DATA_PORT;

  if (ch_status >= USB_FILERR_MIN && ch_status <= USB_FILERR_MAX)
    return ch_status;

  if (ch_status == CH_CMD_RET_SUCCESS)
    return USB_ERR_OK;

  if (ch_status == CH_USB_INT_SUCCESS)
    return USB_ERR_OK;

  if (ch_status == CH_USB_INT_CONNECT)
    return USB_INT_CONNECT;

  if (ch_status == CH_USB_INT_DISK_READ)
    return USB_ERR_DISK_READ;

  if (ch_status == CH_USB_INT_DISK_WRITE)
    return USB_ERR_DISK_WRITE;

  if (ch_status == CH_USB_INT_DISCONNECT) {
    ch_cmd_set_usb_mode(5);
    return USB_ERR_NO_DEVICE;
  }

  if (ch_status == CH_USB_INT_BUF_OVER)
    return USB_ERR_DATA_ERROR;

  ch_status &= 0x2F;

  if (ch_status == 0x2A)
    return USB_ERR_NAK;

  if (ch_status == 0x2E)
    return USB_ERR_STALL;

  ch_status &= 0x23;

  if (ch_status == 0x20)
    return USB_ERR_TIMEOUT;

  if (ch_status == 0x23)
    return USB_TOKEN_OUT_OF_SYNC;

  return USB_ERR_UNEXPECTED_STATUS_FROM_HOST;
}

uint8_t ch_read_data(uint8_t *buffer, uint16_t buffer_size) {
  ch_command(CH_CMD_RD_USB_DATA0);
  const uint8_t amount_received = CH376_DATA_PORT;
  uint8_t       count           = amount_received;
  uint8_t       extra           = 0;

  if (count > buffer_size) {
    extra = count - buffer_size;
    count = buffer_size;
  }

  while (count-- != 0)
    *buffer++ = CH376_DATA_PORT;

  while (extra-- != 0) // do we need to flush buffer?
    CH376_DATA_PORT;

  return amount_received;
}

void ch_cmd_reset_all() { ch_command(CH_CMD_RESET_ALL); }

inline uint8_t ch_cmd_check_exist() {
  uint8_t complement;
  ch_command(CH_CMD_CHECK_EXIST);
  CH376_DATA_PORT = (uint8_t)~0x55;
  complement      = CH376_DATA_PORT;
  if (complement != 0x55)
    return false;

  ch_command(CH_CMD_CHECK_EXIST);
  CH376_DATA_PORT = (uint8_t)~0x89;
  complement      = CH376_DATA_PORT;
  return complement == 0x89;
}

uint8_t ch_probe() {
  uint8_t i = 5;
  do {
    if (ch_cmd_check_exist())
      return true;

    delay_medium();
  } while (--i != 0);

  return false;
}

uint8_t ch_cmd_set_usb_mode(const uint8_t mode) __z88dk_fastcall {
  uint8_t result = 0;

  CH376_COMMAND_PORT = CH_CMD_SET_USB_MODE;
  CH376_DATA_PORT    = mode;

  uint8_t count = 127;

  while (result != CH_CMD_RET_SUCCESS && result != CH_CMD_RET_ABORT && --count != 0)
    result = CH376_DATA_PORT;

  return (result == CH_CMD_RET_SUCCESS) ? USB_ERR_OK : USB_ERR_FAIL;
}

uint8_t ch_cmd_get_ic_version() {
  ch_command(CH_CMD_GET_IC_VER);
  return CH376_DATA_PORT & 0x1f;
}

const uint8_t *ch_write_data(const uint8_t *buffer, uint8_t length) {
  ch_command(CH_CMD_WR_HOST_DATA);
  CH376_DATA_PORT = length;

  while (length-- != 0)
    CH376_DATA_PORT = *buffer++;

  return buffer;
}

void ch_issue_token(const uint8_t toggle_bit, const uint8_t endpoint, const ch376_pid pid) {
  ch_command(CH_CMD_ISSUE_TKN_X);
  CH376_DATA_PORT = toggle_bit;
  CH376_DATA_PORT = endpoint << 4 | pid;
}

void ch_issue_token_in(const endpoint_param *const endpoint) __z88dk_fastcall {
  ch_issue_token(endpoint->toggle ? 0x80 : 0x00, endpoint->number, CH_PID_IN);
}

void ch_issue_token_out(const endpoint_param *const endpoint) __z88dk_fastcall {
  ch_issue_token(endpoint->toggle ? 0x40 : 0x00, endpoint->number, CH_PID_OUT);
}

void ch_issue_token_out_ep0() { ch_issue_token(0x40, 0, CH_PID_OUT); }

void ch_issue_token_in_ep0() { ch_issue_token(0x80, 0, CH_PID_IN); }

void ch_issue_token_setup() { ch_issue_token(0, 0, CH_PID_SETUP); }

usb_error ch_data_in_transfer(uint8_t *buffer, int16_t buffer_size, endpoint_param *const endpoint) {
  uint8_t   count;
  usb_error result;

  if (buffer_size == 0)
    return USB_ERR_OK;

  USB_MODULE_LEDS = 0x01;
  do {
    ch_issue_token_in(endpoint);

    CHECK(ch_long_wait_int_and_get_status());

    endpoint->toggle = !endpoint->toggle;

    count = ch_read_data(buffer, buffer_size);
    if (count == 0) {
      USB_MODULE_LEDS = 0x00;
      return USB_ERR_DATA_ERROR;
    }

    buffer += count;
    buffer_size -= count;
  } while (buffer_size > 0);

  USB_MODULE_LEDS = 0x00;

  return USB_ERR_OK;
}

usb_error ch_data_out_transfer(const uint8_t *buffer, int16_t buffer_length, endpoint_param *const endpoint) {
  usb_error     result;
  const uint8_t number          = endpoint->number;
  const uint8_t max_packet_size = calc_max_packet_size(endpoint->max_packet_sizex);

  USB_MODULE_LEDS = 0x02;

  while (buffer_length > 0) {
    const uint8_t size = max_packet_size < buffer_length ? max_packet_size : buffer_length;
    buffer             = ch_write_data(buffer, size);
    buffer_length -= size;
    ch_issue_token_out(endpoint);

    CHECK(ch_long_wait_int_and_get_status());

    endpoint->toggle = !endpoint->toggle;
  }

  USB_MODULE_LEDS = 0x00;

  return USB_ERR_OK;
}

void ch_set_usb_address(const uint8_t device_address) __z88dk_fastcall {
  ch_command(CH_CMD_SET_USB_ADDR);
  CH376_DATA_PORT = device_address;
}
