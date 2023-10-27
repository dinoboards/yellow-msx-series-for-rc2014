#include "vendor_ftdi.h"

/*  ftdi_to_clkbits Convert a requested baudrate for a given system clock  and predivisor
                    to encoded divisor and the achievable baudrate
    Function is only used internally
    \internal

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
    ftdi_convert_baudrate returns nearest supported baud rate to that requested.
    Function is only used internally
    \internal
*/
static int32_t ftdi_convert_baudrate(int32_t baudrate, uint16_t *value, uint16_t *index) {
  if (baudrate <= 0) {
    trace_printf("  Unsupported baudrate %ld\n", baudrate);
    return -1;
  }

  uint32_t encoded_divisor = 0;
  int32_t  best_baud       = 0;

#define H_CLK 120000000
#define C_CLK 48000000

  best_baud = ftdi_to_clkbits(baudrate, C_CLK, 16, &encoded_divisor);
  // trace_printf("  ftdi_to_clkbits: best_baud=%ld, encoded_divisor=%08lX\n", best_baud, encoded_divisor);

  // Split into "value" and "index" values
  *value = (uint16_t)(encoded_divisor & 0xFFFF);
  *index = (uint16_t)(encoded_divisor >> 16);

  // Return the nearest baud rate
  return best_baud;
}

// setup_packet cmd_set_baudrate = {FTDI_DEVICE_OUT_REQTYPE, SIO_SET_BAUDRATE_REQUEST, {0, 0}, {0, 0}, 0};

usb_error ftdi_set_baudrate(device_config_ftdi *const ftdi, int32_t baudrate) __sdcccall(1) {
  setup_packet cmd;
  usb_error    result;
  uint16_t     value, index;

  if (ftdi->bitbang_enabled)
    baudrate = baudrate * 4;

  const int32_t actual_baudrate = ftdi_convert_baudrate(baudrate, &value, &index);
  trace_printf("  baudrate=%ld, actual_baudrate=%ld\n", baudrate, actual_baudrate);

  if (actual_baudrate <= 0) {
    trace_printf("  bad actual_baudrate %ld\n", baudrate);
    return -1;
  }

  // Check within tolerance (about 5%)
  if ((actual_baudrate * 2 < baudrate /* Catch overflows */) ||
      ((actual_baudrate < baudrate) ? (actual_baudrate * 21 < baudrate * 20) : (baudrate * 21 < actual_baudrate * 20))) {
    trace_printf("  Unsupported baudrate. (%ld, %ld)\n", baudrate, actual_baudrate);
    return -1; // "Unsupported baudrate. Note: bitbang baudrates are automatically multiplied by 4");
  }

  cmd.bmRequestType = FTDI_DEVICE_OUT_REQTYPE;
  cmd.bRequest      = SIO_SET_BAUDRATE_REQUEST;
  cmd.bValue[0]     = value >> 8;
  cmd.bValue[1]     = value & 0xFF;
  cmd.bIndex[0]     = index >> 8;
  cmd.bIndex[1]     = index & 0xFF;
  cmd.wLength       = 0;

  // trace_printf("  v: %04X, i: %04X,  ", value, index);
  // trace_printf("  divisor: %d, clock: %d\n", value & 0x3FFF, ((index & 1) << 2) + (value >> 14));

  result = usbdev_control_transfer((device_config *)ftdi, &cmd, NULL);

  ftdi->baudrate = baudrate;

  return USB_ERR_OK;
}
