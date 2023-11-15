#include <stdbool.h>

#include "baud_rate_clock_calculator.h"
#include <delay.h>
#include <extbio/serial.h>
#include <msxdos.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t wanted_baud_rates[] = {300, 1200, 2400, 4800, 9600, 19200, 38400, 56700, 115200, 230400, 460800, 921600};

#define BUF_SIZE 128

uint8_t read_data_chunk(uint8_t *const read_bytes) {
  uint8_t  result;
  uint8_t  buffer[BUF_SIZE];
  uint16_t buffer_size = BUF_SIZE;

  memset(buffer, 0, sizeof(buffer));

  result      = serial_demand_read(1, buffer, &buffer_size, 1000);
  *read_bytes = buffer_size;

  if (result) {
    printf("Serial read failed %02X\r\n", result);
    return result;
  }

  for (uint8_t i = 0; i < buffer_size; i++)
    printf("%02X  ", buffer[i]);

  return 0;
}

// char     arg1[6];
// char     arg2[6];
// uint16_t idx;
// uint16_t value;

// uint16_t strtoulx(const char *restrict nptr, char **restrict endptr, int base) {
//   // strtoul is broken and corruptes IX!
//   return strtoul((char *)nptr, endptr, base);
// }

uint8_t main(const int argc, const char *const argv[]) {
  (void)argc;
  (void)argv;

  uint8_t result;
  uint8_t port_count;
  char    name[9];
  uint8_t buffer_size = BUF_SIZE;
  uint8_t buffer[BUF_SIZE];
  uint8_t id = 0;

  // printf("argc: %d\r\n", argc);
  // for (int i = 0; i < argc; i++) {
  //   printf("argv[%d]: %s\r\n", i, argv[i]);
  // }

  // memset(arg1, 0, sizeof(arg1));
  // strncpy(arg1, argv[1], 5);
  // memset(arg2, 0, sizeof(arg2));
  // strncpy(arg2, argv[2], 5);

  // printf("arg1: %s, arg2: %s\r\n", arg1, arg2);

  // idx   = strtoulx(arg1, NULL, 16);
  // value = strtoulx(arg2, NULL, 16);

  // printf("Value: %04X\r\n", value);
  // printf("idx %04X,", idx);

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

  result = serial_set_baudrate(1, 2400);
  printf("(%02X): Set baudrate: %ld\r\n", result, 2400L);

  // result = serial_set_flowctrl(1, SERIAL_FLOW_CTRL_RTS_CTS);
  // printf("(%02X): Set flow control: %04X\r\n", result, SERIAL_FLOW_CTRL_RTS_CTS);

  result = serial_set_protocol(1, SERIAL_PARITY_NONE | SERIAL_STOPBITS_1 | SERIAL_BITS_8 | SERIAL_BREAK_OFF);
  printf("(%02X): Set protocol %04X\r\n", result, SERIAL_PARITY_NONE | SERIAL_STOPBITS_1 | SERIAL_BITS_8 | SERIAL_BREAK_OFF);

  result = serial_purge_buffers(1);
  printf("(%02X): Purge buffers\r\n", result);

  uint8_t i = 0;
  while (true) {
    if (msxbiosBreakX())
      return 0;

    for (uint8_t buf_index = 0; buf_index < BUF_SIZE; buf_index++)
      buffer[buf_index] = id++;

    result = serial_write(1, buffer, BUF_SIZE);
    printf("(%02X): serial_write_data %d\r\n", result, BUF_SIZE);
    if (result)
      return result;

    // result = serial_set_dtr_rts(1, SERIAL_SET_DTR_HIGH);
    // printf("(%02X): Set DTR high\r\n", result);

    // result = serial_set_dtr_rts(1, SERIAL_SET_DTR_LOW);
    // printf("(%02X): Set DTR low\r\n", result);

    // result = serial_set_dtr_rts(1, SERIAL_SET_RTS_HIGH);
    // printf("(%02X): Set RTS high\r\n", result);

    // result = serial_set_dtr_rts(1, SERIAL_SET_RTS_LOW);
    // printf("(%02X): Set RTS low\r\n", result);

    uint8_t  read_count;
    uint8_t  total_read = 0;
    uint16_t count      = 0;

    int16_t timeout = get_future_time(from_ms(5000));
    while (total_read < BUF_SIZE && !is_time_past(timeout)) {
      if (msxbiosBreakX())
        return 0;

      result = read_data_chunk(&read_count);
      total_read += read_count;
      count++;
      if (result)
        return result;
    }

    // if (total_read != BUF_SIZE) {
    printf("\r\nserial_read returned %d bytes in %d chunks.  Expect %d bytes\r\n", total_read, count, BUF_SIZE);
    // continue;
    // }

    printf("\r\n");
    // do {
    //   if (msxbiosBreakX())
    //     return 0;

    //   result = read_data_chunk(&read_count);
    //   if (result)
    //     return result;

    //   if (read_count != 0)
    //     printf("\r\nserial_read returned %d bytes.  Expected 0 bytes\r\n", read_count);
    // } while (read_count != 0);
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
