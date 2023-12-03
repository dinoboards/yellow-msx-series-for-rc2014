#include "serial-helpers.h"

uint8_t find_serial_driver(const char *driver_name) __sdcccall(1) {
  uint8_t port_count;
  uint8_t result;
  char    name[9];

  port_count = 0;
  result     = serial_get_available_ports(&port_count);

  for (uint8_t i = 1; i <= port_count; i++) {
    memset(name, 0, sizeof(name));
    result = serial_get_driver_name(i, name);
    if (strcmp(name, driver_name) == 0)
      return i;
  }

  return 0;
}
