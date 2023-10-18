#include "command_printer_check.h"
#include "arguments.h"
#include "device_search.h"
#include <class_printer.h>
#include <delay.h>
#include <msx_fusion.h>
#include <protocol.h>
#include <stdint.h>
#include <stdio.h>

usb_error find_printer(const uint8_t address, device_config_interface *const printer_interface) __sdcccall(1) {
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

      if (interface->bInterfaceClass == 7) {
        printer_interface->address         = address;
        printer_interface->config_index    = config_index;
        printer_interface->interface_index = interface_index;
        return USB_ERR_OK;
      }
    }
  }

  return 255;
}

usb_error command_printer_check(const uint8_t last_device_address) __sdcccall(1) {
  usb_error result;

  device_config_interface printer_interface;
  memset(&printer_interface, 0, sizeof(device_config_interface));

  for (uint8_t address = 1; address <= last_device_address; address++) {
    result = find_printer(address, &printer_interface);

    if (result && result != 255) {
      printf("USB error %02x\r\n", result);
      goto finally;
    }

    if (result == 0) {
      printf("Printer @ address %d, config index %d and interface %d\r\n", printer_interface.address,
             printer_interface.config_index, printer_interface.interface_index);

      device_config_printer printer_config;
      memset(&printer_config, 0, sizeof(printer_config));

      result = construct_device_config(printer_interface.address, printer_interface.config_index, printer_interface.interface_index,
                                       (device_config *)&printer_config);
      if (result) {
        printf("construct_device_config failed %d\r\n", result);
        goto finally;
      }

      const char   *sample_text = "12345678901234567890123456789012345678901234567890123456789012345678901234567890\r\n";
      const uint8_t buffer[128];
      strcpy((char *)buffer, sample_text);

      for (int k = 0; k < 32; k++) {
        result = prt_send_text(&printer_config, buffer, strlen(buffer));

        if (result) {
          printf("prt_send_text failed %d\r\n", result);
          goto finally;
        }
      }
      goto finally;
    }
  }

  result = 0;

finally:
  return result;
}
