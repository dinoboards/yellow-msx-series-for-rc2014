#include "vendor_ftdi.h"
#include <string.h>
#include <usb/ch376.h>

/*  ftdi_to_clkbits Convert a requested baudrate for a given system clock  and predivisor
                    to encoded divisor and the achievable baudrate

   FT232R
   {index[0],value[15:14]} is the encoded subdivisor
*/

static const uint8_t frac_code[8] = {0, 3, 2, 4, 1, 5, 6, 7};

static uint32_t ftdi_to_clkbits(int32_t baudrate, uint32_t clk, uint32_t clk_div, uint32_t *encoded_divisor) {
  int32_t best_baud = 0;
  int32_t divisor, best_divisor;

  if (baudrate >= clk / clk_div) {
    *encoded_divisor = 0;
    best_baud        = clk / clk_div;

  } else if (baudrate >= clk / (clk_div + clk_div / 2)) {
    *encoded_divisor = 1;
    best_baud        = clk / (clk_div + clk_div / 2);

  } else if (baudrate >= clk / (2 * clk_div)) {
    *encoded_divisor = 2;
    best_baud        = clk / (2 * clk_div);

  } else {
    /* We divide by 16 to have 3 fractional bits and one bit for rounding */
    divisor = clk * 16 / clk_div / baudrate;
    if (divisor & 1) /* Decide if to round up or down*/
      best_divisor = divisor / 2 + 1;

    else
      best_divisor = divisor / 2;

    if (best_divisor > 0x20000)
      best_divisor = 0x1ffff;

    best_baud = clk * 16 / clk_div / best_divisor;

    if (best_baud & 1) /* Decide if to round up or down*/
      best_baud = best_baud / 2 + 1;
    else
      best_baud = best_baud / 2;

    *encoded_divisor = (best_divisor >> 3) | ((uint32_t)frac_code[best_divisor & 0x7] << 14);
  }

  return best_baud;
}

/**
 * @brief returns nearest supported baud rate to that requested.
 *
 * @param baudrate the requested baud rate
 * @param value the clock divisor value
 * @param index as per the FTDI chip spec
 * @return int32_t the actually achieved baud rate
 */
static int32_t ftdi_convert_baudrate(int32_t baudrate, uint16_t *value, uint16_t *index) {
  if (baudrate <= 0) {
    trace_printf("  Unsupported baudrate %ld\n", baudrate);
    return -1;
  }

  uint32_t encoded_divisor = 0;
  int32_t  best_baud       = 0;

  best_baud = ftdi_to_clkbits(baudrate, C_CLK, 16, &encoded_divisor);

  *value = (uint16_t)(encoded_divisor & 0xFFFF);
  *index = (uint16_t)(encoded_divisor >> 16);

  return best_baud;
}


/**
 * @brief set the baudrate of the ftdi device
 *
 * @param ftdi the device_config of the ftdi usb device
 * @param baudrate on entry the requested baud rate, on exit the actual baud rate
 * @return usb_error
 */
usb_error ftdi_set_baudrate(device_config_ftdi *const ftdi, int32_t *const baudrate) __sdcccall(1) {
  setup_packet   cmd;
  usb_error      result;
  uint16_t       value, index;
  const uint32_t requested_baudrate = ftdi->bitbang_enabled ? (*baudrate) * 4 : *baudrate;

  const int32_t actual_baudrate = ftdi_convert_baudrate(requested_baudrate, &value, &index);

  if (actual_baudrate <= 0)
    return -1;

  // Check within tolerance (about 5%)
  if ((actual_baudrate * 2 < requested_baudrate /* Catch overflows */) ||
      ((actual_baudrate < requested_baudrate) ? (actual_baudrate * 21 < requested_baudrate * 20)
                                              : (requested_baudrate * 21 < actual_baudrate * 20))) {
    return -1; // "Unsupported baudrate. Note: bitbang baudrates are automatically multiplied by 4");
  }

  cmd.bmRequestType = FTDI_DEVICE_OUT_REQTYPE;
  cmd.bRequest      = SIO_SET_BAUDRATE_REQUEST;
  cmd.bValue[0]     = value >> 8;
  cmd.bValue[1]     = value & 0xFF;
  cmd.bIndex[0]     = index >> 8;
  cmd.bIndex[1]     = index & 0xFF;
  cmd.wLength       = 0;

  result = usbdev_control_transfer((device_config *)ftdi, &cmd, NULL);

  ftdi->baudrate = requested_baudrate;
  *baudrate      = actual_baudrate;

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
usb_error ftdi_set_line_property2(device_config_ftdi *const ftdi,
                                  enum ftdi_bits_type       bits,
                                  enum ftdi_stopbits_type   sbit,
                                  enum ftdi_parity_type     parity,
                                  enum ftdi_break_type      break_type) {
  setup_packet cmd;
  usb_error    result;
  uint16_t     value = bits;

  switch (parity) {
  case NONE:
    value |= (0x00 << 8);
    break;
  case ODD:
    value |= (0x01 << 8);
    break;
  case EVEN:
    value |= (0x02 << 8);
    break;
  case MARK:
    value |= (0x03 << 8);
    break;
  case SPACE:
    value |= (0x04 << 8);
    break;
  }

  switch (sbit) {
  case STOP_BIT_1:
    value |= (0x00 << 11);
    break;
  case STOP_BIT_15:
    value |= (0x01 << 11);
    break;
  case STOP_BIT_2:
    value |= (0x02 << 11);
    break;
  }

  switch (break_type) {
  case BREAK_OFF:
    value |= (0x00 << 14);
    break;
  case BREAK_ON:
    value |= (0x01 << 14);
    break;
  }

  cmd.bmRequestType = FTDI_DEVICE_OUT_REQTYPE;
  cmd.bRequest      = SIO_SET_DATA_REQUEST;
  cmd.bValue[0]     = value >> 8;
  cmd.bValue[1]     = value & 0xFF;
  cmd.bIndex[0]     = 0;
  cmd.bIndex[1]     = 0;
  cmd.wLength       = 0;

  RETURN_CHECK(usbdev_control_transfer((device_config *)ftdi, &cmd, NULL));
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
usb_error ftdi_read_data(device_config_ftdi *const ftdi, uint8_t *const buf, uint8_t *const size) {
  usb_error result;

  if (*size > 64)
    return USB_ERR_BUFF_TO_LARGE;

  uint8_t temp_buf[64];

  CHECK(usbdev_bulk_in_transfer((device_config *)ftdi, temp_buf, size));

  // first 2 bytes are some kind of status code - ignore them
  if (*size <= 2) {
    // no serial bytes received
    *size = 0;
    return USB_ERR_OK;
  }

  *size -= 2;
  memcpy(buf, temp_buf + 2, *size);
  return USB_ERR_OK;
}

usb_error ftdi_write_data(device_config_ftdi *const ftdi, const uint8_t *const buf, const uint16_t size) {
  usb_error result;

  RETURN_CHECK(usbdev_bulk_out_transfer((device_config *)ftdi, buf, size));
}

/**
 * @brief Clears the read buffer on the chip and the internal read buffer.
 *
 */
usb_error ftdi_usb_purge_rx_buffer(device_config_ftdi *const ftdi) __sdcccall(1) {
  setup_packet cmd;
  usb_error    result;

  cmd.bmRequestType = FTDI_DEVICE_OUT_REQTYPE;
  cmd.bRequest      = SIO_RESET_REQUEST;
  cmd.bValue[0]     = SIO_RESET_PURGE_RX >> 8;
  cmd.bValue[1]     = SIO_RESET_PURGE_RX & 0xFF;
  cmd.bIndex[0]     = 0;
  cmd.bIndex[1]     = 0;
  cmd.wLength       = 0;

  RETURN_CHECK(usbdev_control_transfer((device_config *)ftdi, &cmd, NULL));
}

/**
 * @brief Clears the write buffer on the chip.
 *
 */
usb_error ftdi_usb_purge_tx_buffer(device_config_ftdi *const ftdi) __sdcccall(1) {
  setup_packet cmd;
  usb_error    result;

  cmd.bmRequestType = FTDI_DEVICE_OUT_REQTYPE;
  cmd.bRequest      = SIO_RESET_REQUEST;
  cmd.bValue[0]     = SIO_RESET_PURGE_TX >> 8;
  cmd.bValue[1]     = SIO_RESET_PURGE_TX & 0xFF;
  cmd.bIndex[0]     = 0;
  cmd.bIndex[1]     = 0;
  cmd.wLength       = 0;

  RETURN_CHECK(usbdev_control_transfer((device_config *)ftdi, &cmd, NULL));
}
