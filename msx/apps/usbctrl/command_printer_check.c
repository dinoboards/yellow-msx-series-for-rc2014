#include "command_printer_check.h"
#include "arguments.h"
#include "device_search.h"
#include <class_printer.h>
#include <delay.h>
#include <msx_fusion.h>
#include <protocol.h>
#include <stdint.h>
#include <stdio.h>

bool is_printer(const device_descriptor *const device_descriptor, const interface_descriptor *const interface) __sdcccall(1) {
  (void)device_descriptor;
  return (interface->bInterfaceClass == 7);
}

usb_error command_printer_check(const uint8_t last_device_address) __sdcccall(1) {
  usb_error result;

  // device_config_interface printer_interface;
  // memset(&printer_interface, 0, sizeof(device_config_interface));

  for (uint8_t address = 1; address <= last_device_address; address++) {
    device_config_printer printer_config;

    result = find_device(address, is_printer, (device_config *)&printer_config);

    if (result && result != 255) {
      printf("USB error %02x\r\n", result);
      goto finally;
    }

    if (result == 0) {
      // printf("Printer @ address %d, config index %d and interface %d\r\n", printer_interface.address,
      //        printer_interface.config_index, printer_interface.interface_index);

      memset(&printer_config, 0, sizeof(printer_config));

      // result = construct_device_config(printer_interface.address, printer_interface.config_index,
      // printer_interface.interface_index,
      //                                  (device_config *)&printer_config);
      if (result) {
        printf("construct_device_config failed %d\r\n", result);
        goto finally;
      }

      const char   *sample_text = "12345678901234567890123456789012345678901234567890123456789012345678901234567890\r\n";
      const uint8_t buffer[128];
      strcpy((char *)buffer, sample_text);

      for (int k = 0; k < 32; k++) {
        result = prt_send(&printer_config, buffer, strlen(buffer));

        if (result) {
          printf("prt_send failed %d\r\n", result);
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
