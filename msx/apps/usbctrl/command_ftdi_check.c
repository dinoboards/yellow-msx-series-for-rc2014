#include "../sys/ftdi/vendor_ftdi.h"
#include "arguments.h"
#include "command_printer_check.h"
#include "device_search.h"
#include <class_printer.h>
#include <delay.h>
#include <msx_fusion.h>
#include <msxdos.h>
#include <protocol.h>
#include <stdint.h>
#include <stdio.h>
#include <usb/find_device.h>

bool is_ftdi(const device_descriptor *const device_descriptor, const interface_descriptor *const interface) __sdcccall(1) {
  (void)interface;
  return device_descriptor->idVendor == 0x403 && device_descriptor->idProduct == 0x6001 && device_descriptor->bcdDevice == 0x600;
}

#define BUF_SIZE 64

usb_error read_from_ftdi(device_config_ftdi *ftdi_config, uint8_t *const read_bytes) {
  usb_error result;
  uint8_t   buffer[BUF_SIZE];
  uint16_t  buffer_size = BUF_SIZE;

  memset(buffer, 0, sizeof(buffer));

  result = ftdi_read_data(ftdi_config, buffer, &buffer_size);

  if (result == USB_TOKEN_OUT_OF_SYNC) {
    printf("Sync: size: %d, %d, %d\r\n", buffer_size, ftdi_config->endpoints[0].toggle, ftdi_config->endpoints[1].toggle);
    result = ftdi_read_data(ftdi_config, buffer, &buffer_size);
  }

  *read_bytes = buffer_size;

  if (result)
    return result;

  for (uint8_t i = 0; i < buffer_size; i++)
    printf("%02X  ", buffer[i]);

  return USB_ERR_OK;
}

usb_error command_ftdi_check(const uint8_t last_device_address) __sdcccall(1) {
  last_device_address;
  usb_error result = USB_ERR_OK;

  device_config_ftdi ftdi_config;
  memset(&ftdi_config, 0, sizeof(ftdi_config));

  printf("Checking ftdi loop back\r\n");
  result = find_device(is_ftdi, (device_config *)&ftdi_config);

  if (result) {
    printf("No FTDI USB Device Found %02x\r\n", result);
    goto finally;
  }

  printf("ftdi_set_baudrate (requested: %ld", baud_rate);
  result = ftdi_set_baudrate(&ftdi_config, baud_rate);
  if (result)
    printf(")\r\nresult: %d\r\n", result);

  printf(", %ld)\r\n", baud_rate);

  printf("ftdi_set_line_property2: 8 bits, 1 stop bit, no parity, break off\r\n");
  result = ftdi_set_line_property2(&ftdi_config, FTDI_PARITY_NONE | FTDI_STOPBITS_1 | FTDI_BITS_8 | FTDI_BREAK_OFF);
  if (result)
    printf("result: %d\r\n", result);

  printf("ftdi_usb_purge_tx/rx_buffer\r\n");
  result = ftdi_purge_tx_buffer(&ftdi_config);
  if (result)
    printf("ftdi_purge_tx_buffer failed: %d\r\n", result);

  result = ftdi_purge_rx_buffer(&ftdi_config);
  if (result)
    printf("ftdi_purge_rx_buffer failed: %d\r\n", result);

  uint8_t buffer_size = BUF_SIZE;
  uint8_t buffer[BUF_SIZE];
  uint8_t id = 0;

  while (true) {
    if (msxbiosBreakX())
      goto finally;

    printf("ftdi_write_data (64 bytes, from %d)\r\n", id);

    for (uint8_t buf_index = 0; buf_index < BUF_SIZE; buf_index++)
      buffer[buf_index] = id++;

    result = ftdi_write_data(&ftdi_config, buffer, BUF_SIZE);
    if (result) {
      printf("ftdi_write_data errored: %02X\r\n", result);
      break;
    }

    uint16_t status = 0;
    result          = ftdi_poll_modem_status(&ftdi_config, &status);
    printf("ftdi_poll_modem_status: %04X\r\n", result);

    printf("ftdi_read_data: \r\n");
    uint8_t  read_count;
    uint8_t  total_read = 0;
    uint16_t count      = 0;
    int16_t  timeout    = get_future_time(from_ms(5000));
    while (total_read != 64 && !is_time_past(timeout)) {
      if (msxbiosBreakX())
        goto finally;

      result = read_from_ftdi(&ftdi_config, &read_count);
      total_read += read_count;
      count++;
      if (result) {
        printf("ftdi_read_data errored: %d\r\n", result);
        break;
      }
    }

    if (total_read != 64) {
      printf("\r\nftdi_read_data returned %d bytes in %d chunks.  Expected 64 bytes\r\n", total_read, count);
      continue;
    }

    do {
      if (msxbiosBreakX())
        goto finally;

      result = read_from_ftdi(&ftdi_config, &read_count);
      if (result) {
        printf("ftdi_read_data errored: %d\r\n", result);
        break;
      }

      if (read_count != 0)
        printf("\r\nftdi_read_data returned %d bytes.  Expected 0 bytes\r\n", read_count);
    } while (read_count != 0);
  }

finally:
  return result;
}
