#include <stdio.h>

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

#define C_CLK 48000000

void main(const int argc, const char *argv[]) {
  (void)argc;
  (void)argv;
  printf("     Hello World\r\n");

  uint32_t encoded_divisor = 0;
  int32_t  best_baud       = 0;

  best_baud = ftdi_to_clkbits(19200, C_CLK, 16, &encoded_divisor);
  printf("  ftdi_to_clkbits: best_baud=%ld\n", best_baud);

  // Split into "value" and "index" values
  uint16_t value = (uint16_t)(encoded_divisor & 0xFFFF);
  uint16_t index = (uint16_t)(encoded_divisor >> 16);

  printf("index %04X\r\n", index);
  printf("value %04X\r\n", value);

  printf("  divisor: %d, clock: %d\n", value & 0x3FFF, ((index & 1) << 2) + (value >> 14));
}
