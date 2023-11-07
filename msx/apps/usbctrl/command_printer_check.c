#include "command_printer_check.h"
#include "arguments.h"
#include "device_search.h"
#include <delay.h>
#include <msx_fusion.h>
#include <protocol.h>
#include <stdint.h>
#include <stdio.h>
#include <usb/class_printer.h>
#include <usb/find_device.h>

bool is_printer(const device_descriptor *const device_descriptor, const interface_descriptor *const interface) __sdcccall(1) {
  device_descriptor;
  return (interface->bInterfaceClass == 7);
}

usb_error command_printer_check(void) {
  device_config_printer printer_config;

  usb_error result = find_device(is_printer, (device_config *)printer_config);

  if (result == 0) {
    const char   *sample_text = "12345678901234567890123456789012345678901234567890123456789012345678901234567890\r\n";
    const uint8_t buffer[128];
    strcpy((char *)buffer, sample_text);

    for (int k = 0; k < 4; k++) {
      result = prt_send(&printer_config, buffer, strlen(buffer));

      if (result) {
        printf("prt_send failed %d\r\n", result);
        return result;
      }
    }
    return 0;
  }

  printf("USB FTDI Device not found: %02x\r\n", result);
  return result;
}
