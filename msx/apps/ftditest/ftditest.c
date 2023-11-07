#include <stdbool.h>

#include "baud_rate_clock_calculator.h"
#include <extbio/serial.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t wanted_baud_rates[] = {300, 1200, 2400, 4800, 9600, 19200, 38400, 56700, 115200, 230400, 460800, 921600};

uint8_t main(const int argc, const char *const argv[]) {
  (void)argc;
  (void)argv;

  // test via serial interface

  uint8_t port_count = 0;
  uint8_t result     = serial_get_available_ports(&port_count);

  printf("(%02X): Port count: %d\r\n", result, port_count);

  char name[9];
  memset(name, 0, sizeof(name));

  result = serial_get_driver_name(0, name);
  printf("(%02X): Driver name (0): '%s'\r\n", result, name);

  memset(name, 0, sizeof(name));
  result = serial_get_driver_name(port_count, name);
  printf("(%02X): Driver name (1): '%s'\r\n", result, name);

  result = serial_set_baudrate(1, 4800);
  printf("(%02X): Set baudrate: %d\r\n", result, 4800);

  // printf("todo\r\n");

  // uint16_t value;
  // uint16_t index;

  // printf("baud, actual, value, index\r\n");
  // for (uint8_t i = 0; i < sizeof(wanted_baud_rates) / 4; i++) {
  //   uint32_t actual = ftdi_convert_baudrate(wanted_baud_rates[i], &value, &index);

  //   printf("%ld, ", wanted_baud_rates[i]);
  //   printf("%ld, ", actual);
  //   printf("%04X, ", value);
  //   printf("%4X\r\n", index);
  // }

  return 0;
}
