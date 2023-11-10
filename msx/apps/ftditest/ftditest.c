#include <stdbool.h>

#include "baud_rate_clock_calculator.h"
#include <delay.h>
#include <extbio/serial.h>
#include <msxdos.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t wanted_baud_rates[] = {300, 1200, 2400, 4800, 9600, 19200, 38400, 56700, 115200, 230400, 460800, 921600};

#define BUF_SIZE 64

uint8_t read_data_chunk(uint8_t *const read_bytes) {
  uint8_t result;
  uint8_t buffer[BUF_SIZE];
  uint8_t buffer_size = BUF_SIZE;

  memset(buffer, 0, sizeof(buffer));

  result      = serial_read(1, buffer, &buffer_size);
  *read_bytes = buffer_size;

  if (result)
    return result;

  for (uint8_t i = 0; i < buffer_size; i++)
    printf("%02X  ", buffer[i]);

  return 0;
}

uint8_t main(const int argc, const char *const argv[]) {
  (void)argc;
  (void)argv;

  uint8_t result;
  uint8_t port_count;
  char    name[9];
  uint8_t buffer_size = BUF_SIZE;
  uint8_t buffer[BUF_SIZE];
  uint8_t id = 0;

  // test via serial interface

  port_count = 0;
  result     = serial_get_available_ports(&port_count);

  printf("(%02X): Port count: %d\r\n", result, port_count);
  if (port_count < 1)
    return 255;

  memset(name, 0, sizeof(name));
  result = serial_get_driver_name(port_count, name);
  printf("(%02X): Driver name (1): '%s'\r\n", result, name);

  if (strncmp(name, "ftdi", 4) != 0)
    return 255;

  result = serial_set_baudrate(1, 921600);
  printf("(%02X): Set baudrate: %ld\r\n", result, 921600);

  result = serial_set_protocol(1, SERIAL_PARITY_NONE | SERIAL_STOPBITS_1 | SERIAL_BITS_8 | SERIAL_BREAK_OFF);
  printf("(%02X): Set protocol %04X\r\n", result, SERIAL_PARITY_NONE | SERIAL_STOPBITS_1 | SERIAL_BITS_8 | SERIAL_BREAK_OFF);

  result = serial_purge_buffers(1);
  printf("(%02X): Purge buffers\r\n", result);

  while (true) {
    if (msxbiosBreakX())
      return 0;

    for (uint8_t buf_index = 0; buf_index < BUF_SIZE; buf_index++)
      buffer[buf_index] = id++;

    result = serial_write(1, buffer, BUF_SIZE);
    printf("(%02X): serial_write_data\r\n", result);
    if (result)
      return result;

    uint8_t  read_count;
    uint8_t  total_read = 0;
    uint16_t count      = 0;
    int16_t  timeout    = get_future_time(from_ms(5000));
    while (total_read != 64 && !is_time_past(timeout)) {
      if (msxbiosBreakX())
        return 0;

      result = read_data_chunk(&read_count);
      total_read += read_count;
      count++;
      if (result)
        return result;
    }

    // if (total_read != 64) {
    printf("\r\nserial_read returned %d bytes in %d chunks.  Expect 64 bytes\r\n", total_read, count);
    // continue;
    // }

    do {
      if (msxbiosBreakX())
        return 0;

      result = read_data_chunk(&read_count);
      if (result)
        return result;

      if (read_count != 0)
        printf("\r\nserial_read returned %d bytes.  Expected 0 bytes\r\n", read_count);
    } while (read_count != 0);
  }
}

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
