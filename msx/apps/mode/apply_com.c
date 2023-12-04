#include "argument.h"
#include "argument_com.h"
#include <extbio/serial.h>
#include <stdio.h>

uint8_t apply_com_settings(void) {
  uint8_t available_ports = 0;
  serial_get_available_ports(&available_ports);

  printf("Available ports: %d\r\n", available_ports);
  if (port_number > available_ports) {
    printf("Invalid port_number number. Number of available ports are: %d\r\n", available_ports);
    return 255;
  }

  if (baud_rate) {
    printf("port_number %d assigned baud rate of %ld\r\n", port_number, baud_rate);
    uint8_t result = serial_set_baudrate(port_number, baud_rate);
    printf("Result %02X\r\n", result);
  }

  return 0;
}
