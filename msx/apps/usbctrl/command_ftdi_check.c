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
#include <usb/vendor_ftdi.h>

bool is_ftdi(const device_descriptor *const device_descriptor, const interface_descriptor *const interface) __sdcccall(1) {
  (void)interface;
  return device_descriptor->idVendor == 0x403 && device_descriptor->idProduct == 0x6001 && device_descriptor->bcdDevice == 0x600;
}

usb_error command_ftdi_check(void) {
  usb_error result = USB_ERR_OK;

  device_config_ftdi ftdi_config;
  memset(&ftdi_config, 0, sizeof(ftdi_config));

  printf("Checking ftdi loop back\r\n");
  result = find_device(is_ftdi, (device_config *)&ftdi_config);

  if (result) {
    printf("No FTDI USB Device Found %02x\r\n", result);
    return result;
  }

  printf("ftdi_set_baudrate(2400):\r\n");
  result = ftdi_set_baudrate(&ftdi_config, 2400);
  if (result)
    printf("result: %d\r\n", result);

  printf("ftdi_set_baudrate(4800):\r\n");
  result = ftdi_set_baudrate(&ftdi_config, 4800);
  if (result)
    printf("result: %d\r\n", result);

  printf("ftdi_set_baudrate(9600):\r\n");
  result = ftdi_set_baudrate(&ftdi_config, 9600);
  if (result)
    printf("result: %d\r\n", result);

  printf("ftdi_set_baudrate(14400):\r\n");
  result = ftdi_set_baudrate(&ftdi_config, 14400);
  if (result)
    printf("result: %d\r\n", result);

  printf("ftdi_set_baudrate(19200):\r\n", result);
  result = ftdi_set_baudrate(&ftdi_config, 19200);
  if (result)
    printf("result: %d\r\n", result);

  printf("ftdi_set_baudrate(38400):\r\n", result);
  result = ftdi_set_baudrate(&ftdi_config, 38400);
  if (result)
    printf("result: %d\r\n", result);

  printf("ftdi_set_baudrate(57600):\r\n", result);
  result = ftdi_set_baudrate(&ftdi_config, 57600);
  if (result)
    printf("result: %d\r\n", result);

  return 0;
}
