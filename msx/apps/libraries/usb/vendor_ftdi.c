#include "vendor_ftdi.h"
#include <string.h>
#include <usb/ch376.h>

setup_packet cmd_ftdi_out_request = {FTDI_DEVICE_OUT_REQTYPE, SIO_RESET_REQUEST, {0, 0}, {0, 0}, 0};

#define BAUD_RATES_COUNT 12

baud_rate_clock_divisor baud_rate_clock_divisors[BAUD_RATES_COUNT] = {
    {300, 300, 0x2710, 0},       {1200, 1200, 0x09C4, 0},     {2400, 2400, 0x04E2, 0},     {4800, 4800, 0x0271, 0},
    {9600, 9600, 0x4138, 0},     {19200, 19200, 0x809C, 0},   {38400, 38400, 0xC04E, 0},   {56700, 56738, 0xC034, 1},
    {115200, 115385, 0x001A, 0}, {230400, 230769, 0x000D, 0}, {460800, 461538, 0x4006, 0}, {921600, 923077, 0x8003, 0},
};

/**
 * @brief returns nearest supported baud rate to that requested.
 *
 * @param baudrate the requested baud rate
 * @param value the clock divisor value
 * @param index as per the FTDI chip spec
 * @return int32_t the actually achieved baud rate
 */

static bool ftdi_convert_baudrate(const int32_t baudrate, setup_packet *const cmd) {
  baud_rate_clock_divisor             *p           = baud_rate_clock_divisors;
  const baud_rate_clock_divisor *const end_address = &baud_rate_clock_divisors[BAUD_RATES_COUNT];

  do {
    if (p->requested_baud_rate == baudrate) {
      cmd->bValue[0] = p->value >> 8;
      cmd->bValue[1] = p->value & 0xFF;
      cmd->bIndex[0] = p->actual_baud_rate >> 8;
      cmd->bIndex[1] = p->actual_baud_rate & 0xFF;
      return true;
    }
    p++;
  } while (p < end_address);

  return false;
}

/**
 * @brief set the baudrate of the ftdi device
 *
 * @param ftdi the device_config of the ftdi usb device
 * @param baudrate on entry the requested baud rate, on exit the actual baud rate
 * @return usb_error
 */
usb_error ftdi_set_baudrate(device_config_ftdi *const ftdi, const int32_t baudrate) {
  setup_packet cmd;
  memcpy(&cmd, &cmd_ftdi_out_request, sizeof(setup_packet));
  usb_error      result;
  const uint32_t requested_baudrate = ftdi->bitbang_enabled ? (baudrate * 4) : baudrate;

  if (!ftdi_convert_baudrate(requested_baudrate, &cmd))
    return USB_ERR_FAIL;

  cmd.bRequest = SIO_SET_BAUDRATE_REQUEST;

  CHECK(usbdev_control_transfer((device_config *)ftdi, &cmd, NULL));

  ftdi->baudrate = requested_baudrate;

  return USB_ERR_OK;
}

/**
 * @brief Set (RS232) line characteristics
 *
 * @param ftdi the device_config of the ftdi usb device
 * @param bits number of data bits
 * @param sbit number of stop bits
 * @param parity parity mode
 * @param break_type break type
 * @return usb_error
 */

usb_error ftdi_set_line_property2(device_config_ftdi *const ftdi, const uint16_t protocol_bits) {
  setup_packet cmd;
  memcpy(&cmd, &cmd_ftdi_out_request, sizeof(setup_packet));

  cmd.bmRequestType = FTDI_DEVICE_OUT_REQTYPE;
  cmd.bValue[0]     = protocol_bits >> 8;
  cmd.bValue[1]     = protocol_bits & 0xFF;

  return usbdev_control_transfer((device_config *)ftdi, &cmd, NULL);
}

/**
 * @brief retrieve any pending data in the rx bufffer
 *
 * @param ftdi the device_config of the ftdi usb device
 * @param buf the buffer to store the received bytes in
 * @param size the size of the buffer - must be no more than 64 bytes
 *             return the number of bytes actually received
 * @return usb_error
 */
usb_error
_ftdi_read_data(xxx *const tmp) __z88dk_fastcall { // tmpdevice_config_ftdi *const ftdi, uint8_t *const buf, uint8_t *const size) {
  usb_error result;

  if (*tmp->size > 64)
    return USB_ERR_BUFF_TO_LARGE;

  uint8_t temp_buf[64];

  CHECK(usbdev_bulk_in_transfer((device_config *)tmp->ftdi, temp_buf, tmp->size));

  // first 2 bytes are some kind of status code - ignore them
  if (*tmp->size <= 2) {
    // no serial bytes received
    *tmp->size = 0;
    return USB_ERR_OK;
  }

  *tmp->size -= 2;
  memcpy(tmp->buf, temp_buf + 2, *tmp->size);
  return USB_ERR_OK;
}

/**
 * @brief Clears the read buffer on the chip and the internal read buffer.
 *
 */
usb_error ftdi_purge_rx_buffer(device_config_ftdi *const ftdi) {
  setup_packet cmd;
  memcpy(&cmd, &cmd_ftdi_out_request, sizeof(setup_packet));

  cmd.bValue[0] = SIO_RESET_PURGE_RX >> 8;
  cmd.bValue[1] = SIO_RESET_PURGE_RX & 0xFF;

  return usbdev_control_transfer((device_config *)ftdi, &cmd, NULL);
}

/**
 * @brief Clears the write buffer on the chip.
 *
 */
usb_error ftdi_purge_tx_buffer(device_config_ftdi *const ftdi) {
  setup_packet cmd;
  memcpy(&cmd, &cmd_ftdi_out_request, sizeof(setup_packet));

  cmd.bValue[0] = SIO_RESET_PURGE_TX >> 8;
  cmd.bValue[1] = SIO_RESET_PURGE_TX & 0xFF;

  return usbdev_control_transfer((device_config *)ftdi, &cmd, NULL);
}
