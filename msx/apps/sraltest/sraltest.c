#include <stdbool.h>

#include "../sys/sio2/sio2.h"
#include "baud_rate_clock_calculator.h"
#include <delay.h>
#include <extbio/serial-helpers.h>
#include <extbio/serial.h>
#include <msxdos.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t wanted_baud_rates[] = {300, 1200, 2400, 4800, 9600, 19200, 38400, 56700, 115200, 230400, 460800, 921600};

#define BUF_SIZE 256

uint8_t port_number = 0;
uint8_t serial_write_temp_buffer[256];

uint8_t read_data_chunk(uint16_t *const read_bytes) {
  uint8_t  result;
  uint8_t  buffer[BUF_SIZE];
  uint16_t buffer_size = BUF_SIZE;

  memset(buffer, 0, sizeof(buffer));

  uint16_t pending_bytes = serial_get_rx_buffer_size(port_number);

  sio_diagnostic_t t;
  serial_get_diagnostics(port_number, &t);
  printf("sio_buf: %04X\r\n", t.sio_buf);
  printf("sio_buf_head: %04X\r\n", t.sio_buf_head);
  printf("sio_buf_tail: %04X\r\n", t.sio_buf_tail);
  printf("sio_data_count: %04X\r\n", t.sio_data_count);
  printf("ch: %04X\r\n", t.ch);

  result      = serial_demand_read(port_number, buffer, &buffer_size, 1000);
  *read_bytes = buffer_size;

  if (result) {
    printf("Serial read failed %02X\r\n", result);
  }

  printf("%04X bytes: ", pending_bytes);

  for (uint8_t i = 0; i < buffer_size; i++)
    printf("%c", buffer[i]);

  printf("\r\n");

  pending_bytes = serial_get_rx_buffer_size(port_number);
  printf("bytes pending: %04X\r\n", pending_bytes);

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

void atok(void) {
  char     buffer[16];
  uint16_t read_count;

  uint8_t result = serial_write_string("at\r\n");
  printf("(%02X): serial_write_string: at\r\n", result);

  read_count = 4;
  memset(buffer, 0, sizeof(buffer));
  result = serial_demand_read(port_number, buffer, &read_count, 1000);
  printf("(%02X): serial_read returned %d bytes. \"%s\" \r\n", result, read_count, buffer);
}

uint8_t main(const int argc, const char *const argv[]) {
  (void)argc;
  (void)argv;

  sio_diagnostic_t d;
  memset(&d, 0, sizeof(d));

  uint8_t result;
  // uint16_t  buffer_size = BUF_SIZE;
  uint8_t  buffer[BUF_SIZE];
  uint8_t  id = 0;
  uint16_t read_count;

  list_serial_drivers();

  if (argc != 2) {
    printf("Usage: sraltest <driver name>\r\n");
    return 1;
  }

  port_number = find_serial_driver(argv[1]);

  printf("%s driver id: %d\r\n", argv[1], port_number);

  if (port_number == 0)
    return 1;

  result = serial_demand_read(port_number, buffer, &read_count, 1000);

  printf("(%02X): serial_read(2nd) returned %d bytes.  Expect 0 bytes\r\n", result, read_count);

  result = serial_set_baudrate(port_number, 19200);
  printf("(%02X): Set baudrate: %ld\r\n", result, 19200L);

  result = serial_set_protocol(port_number, SERIAL_PARITY_NONE | SERIAL_STOPBITS_1 | SERIAL_BITS_8 | SERIAL_BREAK_OFF);
  printf("(%02X): Set protocol %04X\r\n", result, SERIAL_PARITY_NONE | SERIAL_STOPBITS_1 | SERIAL_BITS_8 | SERIAL_BREAK_OFF);

  result = serial_purge_buffers(1);
  printf("(%02X): Purge buffers\r\n", result);

  uint8_t i = 0;
  if (msxbiosBreakX())
    return 0;

  for (uint16_t buf_index = 0; buf_index < BUF_SIZE; buf_index++)
    buffer[buf_index] = id++;

  uint16_t pending_bytes = serial_get_rx_buffer_size(port_number);
  printf("pending: %04X\r\n", pending_bytes);

  sio_diagnostic_t t;
  serial_get_diagnostics(port_number, &t);
  printf("sio_buf: %04X\r\n", t.sio_buf);
  printf("sio_buf_head: %04X\r\n", t.sio_buf_head);
  printf("sio_buf_tail: %04X\r\n", t.sio_buf_tail);
  printf("sio_data_count: %04X\r\n", t.sio_data_count);
  printf("ch: %04X\r\n", t.ch);

  result = serial_write_string("ate0\r\n");
  printf("(%02X): serial_write_string: ate0\r\n", result);
  if (result)
    return result;

  read_count = 100;
  memset(buffer, 0, sizeof(buffer));
  result = serial_demand_read(port_number, buffer, &read_count, 1000);
  printf("(%02X): serial_read returned %d bytes. \"%s\" \r\n", result, read_count, buffer);

  result = serial_write_string("at+baud=9600\r\n");
  printf("(%02X): serial_write_string at+baud=9600\r\n", result);
  if (result)
    return result;

  result = serial_set_baudrate(port_number, 9600L);
  printf("(%02X): Set baudrate: %ld\r\n", result, 9600L);

  read_count = 4;
  memset(buffer, 0, sizeof(buffer));
  result = serial_demand_read(port_number, buffer, &read_count, 1000);
  printf("(%02X): serial_read returned %d bytes. \"%s\" \r\n", result, read_count, buffer);

  atok();
  atok();
  atok();
  atok();

  result = serial_write_string("at+wgethttp://192.168.86.182:8080/bob.txt\r\n");
  printf("(%02X): serial_write_string\r\n", result);
  if (result)
    return result;

  read_count = 4;
  memset(buffer, 0, sizeof(buffer));
  result = serial_demand_read(port_number, buffer, &read_count, 1000);
  printf("(%02X): serial_read returned %d bytes. \"%s\" \r\n", result, read_count, buffer);

  result = serial_write_char('C');
  printf("(%02X): serial_write_char\r\n", result);

  pending_bytes = serial_get_rx_buffer_size(port_number);
  printf("pending: %04X\r\n", pending_bytes);

  result = serial_read_char(buffer);
  printf("(%02X): serial_read_char returned %c\r\n", result, buffer[0]);

  pending_bytes = serial_get_rx_buffer_size(port_number);
  printf("pending: %04X\r\n", pending_bytes);

  memset(buffer, 0, sizeof(buffer));
  read_count = 160;
  result     = serial_read(port_number, buffer, &read_count);
  printf("(%02X): serial_deman_read returned %d bytes.  Expect 132 bytes\r\n", result, read_count);

  for (uint16_t i = 0; i < read_count; i++)
    printf("%02X ", buffer[i]);

  printf("\r\n");

  serial_get_diagnostics(port_number, &t);
  printf("sio_buf: %04X\r\n", t.sio_buf);
  printf("sio_buf_head: %04X\r\n", t.sio_buf_head);
  printf("sio_buf_tail: %04X\r\n", t.sio_buf_tail);
  printf("sio_data_count: %04X\r\n", t.sio_data_count);
  printf("ch: %04X\r\n", t.ch);

  return 0;
}
