#include <stdbool.h>

#include "baud_rate_clock_calculator.h"
#include <delay.h>
#include <extbio/serial.h>
#include <msxdos.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../sys/sio2/sio2.h"

uint32_t wanted_baud_rates[] = {300, 1200, 2400, 4800, 9600, 19200, 38400, 56700, 115200, 230400, 460800, 921600};

#define BUF_SIZE 64

uint8_t driver_id = 0;

uint8_t read_data_chunk(uint16_t *const read_bytes) {
  uint8_t  result;
  uint8_t  buffer[BUF_SIZE];
  uint16_t buffer_size = BUF_SIZE;

  memset(buffer, 0, sizeof(buffer));

  uint16_t pending_bytes = serial_get_rx_buffer_size(driver_id);

  result      = serial_demand_read(driver_id, buffer, &buffer_size, 1000);
  *read_bytes = buffer_size;

  if (result) {
    printf("Serial read failed %02X\r\n", result);
    return result;
  }

  printf("%04X bytes: ", pending_bytes);

  for (uint8_t i = 0; i < buffer_size; i++)
    printf("%02X  ", buffer[i]);

  printf("\r\n");

  pending_bytes = serial_get_rx_buffer_size(driver_id);
  printf("Status: %04X\r\n", pending_bytes);

  return 0;
}

void list_serial_drivers(void) {
  uint8_t port_count;
  uint8_t result;
  char    name[9];

  port_count = 0;
  result     = serial_get_available_ports(&port_count);
  printf("(%02X): Port count: %d\r\n", result, port_count);

  for (uint8_t i = 1; i <= port_count; i++) {
    memset(name, 0, sizeof(name));
    result = serial_get_driver_name(i, name);
    printf("(%02X): Driver name (%d): '%s'\r\n", result, i, name);
  }
}

uint8_t find_serial_driver(const char *driver_name) {
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

uint8_t main(const int argc, const char *const argv[]) {
  (void)argc;
  (void)argv;

  sio_diagnostic_t d;
  memset(&d, 0, sizeof(d));

  uint8_t  result;
  uint8_t  buffer_size = BUF_SIZE;
  uint8_t  buffer[BUF_SIZE];
  uint8_t  id = 0;
  uint16_t read_count;

  list_serial_drivers();

  if (argc != 2) {
    printf("Usage: sraltest <driver name>\r\n");
    return 1;
  }

  driver_id = find_serial_driver(argv[1]);

  printf("%s driver id: %d\r\n", argv[1], driver_id);

  if (driver_id == 0)
    return 1;

  result = serial_demand_read(driver_id, buffer, &read_count, 1000);

  printf("(%02X): serial_read(2nd) returned %d bytes.  Expect 0 bytes\r\n", result, read_count);

  result = serial_set_baudrate(driver_id, 19200);
  printf("(%02X): Set baudrate: %ld\r\n", result, 19200L);

  // result = serial_set_flowctrl(1, SERIAL_FLOW_CTRL_RTS_CTS);
  // printf("(%02X): Set flow control: %04X\r\n", result, SERIAL_FLOW_CTRL_RTS_CTS);

  result = serial_set_protocol(driver_id, SERIAL_PARITY_NONE | SERIAL_STOPBITS_1 | SERIAL_BITS_8 | SERIAL_BREAK_OFF);
  printf("(%02X): Set protocol %04X\r\n", result, SERIAL_PARITY_NONE | SERIAL_STOPBITS_1 | SERIAL_BITS_8 | SERIAL_BREAK_OFF);

  result = serial_purge_buffers(1);
  printf("(%02X): Purge buffers\r\n", result);

  uint8_t i = 0;
  while (true) {
    if (msxbiosBreakX())
      return 0;

    for (uint8_t buf_index = 0; buf_index < BUF_SIZE; buf_index++)
      buffer[buf_index] = id++;

    uint16_t pending_bytes = serial_get_rx_buffer_size(driver_id);
    printf("Status: %04X\r\n", pending_bytes);

    result = serial_write(driver_id, "\r\nAT+msxrc2014\r\n", 6);
    printf("(%02X): serial_write_data %d\r\n", result, 6);
    if (result)
      return result;

    result = read_data_chunk(&read_count);

    printf("(%02X): serial_read returned %d bytes.  Expect %d bytes\r\n", result, read_count, BUF_SIZE);

    printf("\r\n");

    result = serial_demand_read(driver_id, buffer, &read_count, 1000);

    printf("(%02X): serial_read(2nd) returned %d bytes.  Expect 0 bytes\r\n", result, read_count);

    printf("\r\n");

    const int16_t timeout = get_future_time(from_ms(2000));
    while (!is_time_past(timeout)) {
      if (msxbiosBreakX())
        return 0;
    }
  }
}
