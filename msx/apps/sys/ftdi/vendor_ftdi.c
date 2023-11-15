#include "vendor_ftdi.h"
#include <delay.h>
#include <string.h>
#include <usb/ch376.h>
#include <z80.h>

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
      cmd->bValue[0] = p->value & 0xFF;
      cmd->bValue[1] = p->value >> 8;
      cmd->bIndex[0] = p->index & 0xFF;
      cmd->bIndex[1] = p->index >> 8;
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

usb_error ftdi_set_clks(device_config_ftdi *const ftdi, const uint16_t value, const uint16_t index) {
  setup_packet cmd;
  memcpy(&cmd, &cmd_ftdi_out_request, sizeof(setup_packet));
  usb_error result;

  cmd.bValue[0] = value >> 8;
  cmd.bValue[1] = value & 0xFF;
  cmd.bIndex[0] = index >> 8;
  cmd.bIndex[1] = index & 0xFF;
  cmd.bRequest  = SIO_SET_BAUDRATE_REQUEST;

  RETURN_CHECK(usbdev_control_transfer((device_config *)ftdi, &cmd, NULL));
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
  cmd.bValue[1]     = protocol_bits >> 8;
  cmd.bValue[0]     = protocol_bits & 0xFF;

  return usbdev_control_transfer((device_config *)ftdi, &cmd, NULL);
}

usb_error __ftdi_read_packet(device_config_ftdi *const ftdi, uint8_t *const buf, uint8_t *const size) __sdcccall(1) {
  usb_error result;

  if (ftdi->hold_size > 0) {
    memcpy(buf, &ftdi->hold.data, ftdi->hold_size);
    *size           = ftdi->hold_size;
    ftdi->hold_size = 0;
    return USB_ERR_OK;
  }

  if (*size > 62)
    return USB_ERR_BUFF_TO_LARGE;

  CHECK(usbdev_bulk_in_transfer((device_config *)ftdi, ftdi->hold.status, &ftdi->hold_size));

  // ignore the first 2 status bytes
  if (ftdi->hold_size <= 2) {
    *size           = 0;
    ftdi->hold_size = 0;
    return USB_ERR_OK;
  }

  ftdi->hold_size -= 2;

  if (*size > ftdi->hold_size) {
    memcpy(buf, ftdi->hold.data, ftdi->hold_size);
    *size           = ftdi->hold_size;
    ftdi->hold_size = 0;

  } else {
    const uint8_t remaining_bytes = ftdi->hold_size - *size;
    memcpy(buf, ftdi->hold.data, *size);
    memcpy(ftdi->hold.data, ftdi->hold.data + *size, remaining_bytes);
    ftdi->hold_size -= *size;
  }

  return USB_ERR_OK;
}

/**
 * @brief retrieve any pending data in the rx bufffer
 *
 * @param ftdi the device_config of the ftdi usb device
 * @param buf the buffer to store the received bytes in - must be 62 bytes
 * @param size returns the number of bytes actually received
 * @return usb_error
 */
usb_error ftdi_read_data(device_config_ftdi *const ftdi, uint8_t *buf, uint16_t *const size) __sdcccall(1) {
  usb_error result;
  uint8_t   pck_size;
  uint16_t  actual_size = 0;

  do {
    pck_size = *size - actual_size;
    if (pck_size > 62)
      pck_size = 62;
    CHECK(__ftdi_read_packet(ftdi, buf, &pck_size));
    actual_size += pck_size;
    buf += pck_size;
  } while (pck_size != 0 && actual_size < *size);

  *size = actual_size;

  return USB_ERR_OK;
}

/**
 * @brief retrieve any pending data in the rx bufffer
 *
 * @param ftdi the device_config of the ftdi usb device
 * @param buf the buffer to store the received bytes in - must be 62 bytes
 * @param size returns the number of bytes actually received
 * @return usb_error
 */
usb_error ftdi_demand_read_data(device_config_ftdi *const ftdi, uint8_t *buf, uint16_t *const size, const uint16_t timeout_ms)
    __sdcccall(1) {
  usb_error result;
  uint8_t   pck_size;
  uint16_t  actual_size = 0;

  int16_t timeout = get_future_time(from_ms(timeout_ms));
  do {
    EI;
    pck_size = *size - actual_size;
    if (pck_size > 62)
      pck_size = 62;
    CHECK(__ftdi_read_packet(ftdi, buf, &pck_size));
    actual_size += pck_size;
    buf += pck_size;
  } while (actual_size < *size && !is_time_past(timeout));

  *size = actual_size;

  return USB_ERR_OK;
}

/**
 * @brief Clears the read buffer on the chip and the internal read buffer.
 *
 */
usb_error ftdi_purge_rx_buffer(device_config_ftdi *const ftdi) {
  setup_packet cmd;
  memcpy(&cmd, &cmd_ftdi_out_request, sizeof(setup_packet));

  cmd.bValue[1] = SIO_RESET_PURGE_RX >> 8;
  cmd.bValue[0] = SIO_RESET_PURGE_RX & 0xFF;

  return usbdev_control_transfer((device_config *)ftdi, &cmd, NULL);
}

/**
 * @brief Clears the write buffer on the chip.
 *
 */
usb_error ftdi_purge_tx_buffer(device_config_ftdi *const ftdi) {
  setup_packet cmd;
  memcpy(&cmd, &cmd_ftdi_out_request, sizeof(setup_packet));

  cmd.bValue[1] = SIO_RESET_PURGE_TX >> 8;
  cmd.bValue[0] = SIO_RESET_PURGE_TX & 0xFF;

  return usbdev_control_transfer((device_config *)ftdi, &cmd, NULL);
}

/**
    Set flowcontrol for ftdi chip

    \param ftdi pointer to ftdi_context
    \param flowctrl flow control to use. should be
           SIO_DISABLE_FLOW_CTRL, SIO_RTS_CTS_HS, SIO_DTR_DSR_HS or SIO_XON_XOFF_HS

    \retval  0: all fine
    \retval -1: set flow control failed
    \retval -2: USB device unavailable
*/

/**
 * @brief Set flowcontrol for ftdi chip
 *
 * @param ftdi
 * @param flowctrl one of SIO_DISABLE_FLOW_CTRL, SIO_RTS_CTS_HS, SIO_DTR_DSR_HS or SIO_XON_XOFF_HS
 * @return usb_error
 */
usb_error ftdi_set_flowctrl(device_config_ftdi *const ftdi, const uint8_t flowctrl) {
  setup_packet cmd;
  memset(&cmd, 0, sizeof(setup_packet));
  cmd.bmRequestType = FTDI_DEVICE_OUT_REQTYPE;
  cmd.bRequest      = SIO_SET_FLOW_CTRL_REQUEST;
  cmd.bIndex[0]     = 0;
  cmd.bIndex[1]     = flowctrl;

  return usbdev_control_transfer((device_config *)ftdi, &cmd, NULL);
}

/**
 * @brief Set dtr and/or rts line
 *
 * @param ftdi
 * @param dtr_rts_flags combindation of SIO_SET_DTR_HIGH/LOW and/or SIO_SET_RTS_HIGH/LOW
 * @return usb_error
 */
usb_error ftdi_set_dtr_rts(device_config_ftdi *const ftdi, const uint16_t dtr_rts_flags) {
  setup_packet cmd;
  memset(&cmd, 0, sizeof(setup_packet));
  cmd.bmRequestType = FTDI_DEVICE_OUT_REQTYPE;
  cmd.bRequest      = SIO_SET_MODEM_CTRL_REQUEST;
  cmd.bValue[0]     = dtr_rts_flags & 0xFF;
  cmd.bValue[1]     = dtr_rts_flags >> 8;

  return usbdev_control_transfer((device_config *)ftdi, &cmd, NULL);
}
