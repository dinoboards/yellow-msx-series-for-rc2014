#include "arguments.h"
#include "command_printer_check.h"
#include "device_search.h"
#include <class_printer.h>
#include <delay.h>
#include <msx_fusion.h>
#include <protocol.h>
#include <stdint.h>
#include <stdio.h>
#include <usb/vendor_ftdi.h>

usb_error find_ftdi(const uint8_t address, device_config_interface *const ftdi_interface) __sdcccall(1) {
  uint8_t buffer[151];

  device_descriptor my_device_descriptor;
  memset(&my_device_descriptor, 0, sizeof(device_descriptor));

  usb_error result = usbtrn_get_descriptor2(&my_device_descriptor, address);
  __asm__("EI");
  if (result) {
    printf("usbtrn_get_descriptor2 failed %d\r\n", result);
    return result;
  }

  for (uint8_t config_index = 0; config_index < my_device_descriptor.bNumConfigurations; config_index++) {
    memset(buffer, 0, sizeof(buffer));
    result = usbtrn_get_full_config_descriptor(config_index, address, my_device_descriptor.bMaxPacketSize0, 150, buffer);
    __asm__("EI");
    if (result) {
      printf("usbtrn_get_full_config_descriptor failed %d\r\n", result);
      return result;
    }

    const config_descriptor *const config = (config_descriptor *)buffer;

    for (uint8_t interface_index = 0; interface_index < config->bNumInterfaces; interface_index++) {
      const interface_descriptor *const interface =
          (interface_descriptor *)(buffer + sizeof(config_descriptor) + interface_index * sizeof(interface_descriptor));

      if (my_device_descriptor.idVendor == 0x403 && my_device_descriptor.idProduct == 0x6001 &&
          my_device_descriptor.bcdDevice == 0x600) {
        ftdi_interface->address         = address;
        ftdi_interface->config_index    = config_index;
        ftdi_interface->interface_index = interface_index;
        return USB_ERR_OK;
      }
    }
  }

  return 255;
}

usb_error command_ftdi_check(const uint8_t last_device_address) __sdcccall(1) {
  usb_error result;

  device_config_interface ftdi_interface;
  memset(&ftdi_interface, 0, sizeof(device_config_interface));

  for (uint8_t address = 1; address <= last_device_address + 1; address++) {
    result = find_ftdi(address, &ftdi_interface);

    if (result && result != 255) {
      printf("USB error %02x\r\n", result);
      goto finally;
    }

    if (result == 0) {
      printf("FTDI @ address %d, config index %d and interface %d\r\n", ftdi_interface.address, ftdi_interface.config_index,
             ftdi_interface.interface_index);

      device_config_ftdi ftdi_config;
      memset(&ftdi_config, 0, sizeof(ftdi_config));

      result = construct_device_config(ftdi_interface.address, ftdi_interface.config_index, ftdi_interface.interface_index,
                                       (device_config *)&ftdi_config);
      if (result) {
        printf("construct_device_config failed %d\r\n", result);
        goto finally;
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

      goto finally;
    }
  }

  result = 0;

finally:
  return result;
}
