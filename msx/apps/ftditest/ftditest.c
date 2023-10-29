#include "baud_rate_clock_calculator.h"
#include <stdint.h>
#include <stdio.h>

uint32_t wanted_baud_rates[] = {300, 1200, 2400, 4800, 9600, 19200, 38400, 56700, 115200, 230400, 460800, 921600};

uint8_t main(const int argc, const char *const argv[]) {
  (void)argc;
  (void)argv;
  printf("todo\r\n");

  uint16_t value;
  uint16_t index;

  printf("baud, actual, value, index\r\n");
  for (uint8_t i = 0; i < sizeof(wanted_baud_rates) / 4; i++) {
    uint32_t actual = ftdi_convert_baudrate(wanted_baud_rates[i], &value, &index);

    printf("%ld, ", wanted_baud_rates[i]);
    printf("%ld, ", actual);
    printf("%04X, ", value);
    printf("%4X\r\n", index);
  }

  return 0;
}
