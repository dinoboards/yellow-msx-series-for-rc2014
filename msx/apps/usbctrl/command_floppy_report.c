#include "command_floppy_report.h"
#include "device_search.h"

usb_error command_floppy_report(const uint8_t last_device_address) __sdcccall(1) {
  device_config_interface floppy_interface;
  memset(&floppy_interface, 0, sizeof(device_config_interface));

  for (uint8_t address = 1; address <= last_device_address; address++) {

    usb_error result = find_next_floppy_interface(address, &floppy_interface);

    if (result && result != 255) {
      printf("USB error %02x\r\n", result);
      return result;
    }

    if (result == 0)
      printf("Floppy @ address %d, config index %d and interface %d\r\n", floppy_interface.address, floppy_interface.config_index,
             floppy_interface.interface_index);
  }

  return 0;
}
