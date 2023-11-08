#include "../ftdi.h"
#include <stdbool.h>
#include <stdio.h>
#include <sysload/sysload.h>
#include <usb/find_device.h>
#include <usb/protocol.h>
#include <usb/vendor_ftdi.h>

extern device_config_ftdi _ftdi_config;
device_config_ftdi       *ftdi_config;

void serial_get_available_ports(uint8_t *const available_ports) __z88dk_fastcall;

#define relocated(x)  ((void *)((uint16_t)(x) + (uint16_t)(my_header)))
#define relocated2(x) ((uint8_t *)((uint16_t)(x) + (uint16_t)(my_header)))

bool is_ftdi(const device_descriptor *const device_descriptor, const interface_descriptor *const interface) __sdcccall(1) {
  (void)interface;
  return device_descriptor->idVendor == 0x403 && device_descriptor->idProduct == 0x6001 && device_descriptor->bcdDevice == 0x600;
}

uint16_t init(installed_sys_t *const my_header) __z88dk_fastcall {
  ftdi_config      = ((device_config_ftdi *)relocated(_ftdi_config));
  usb_error result = find_device(is_ftdi, (device_config *)ftdi_config);

  if (result != 0) {
    uint8_t *const relocated_assigned_port_number = &relocated2(&assigned_port_number);
    serial_get_available_ports(relocated_assigned_port_number);
    *relocated_assigned_port_number += 1;

    printf("FTDI.SYS:        INSTALLED (COM%d)\r\n", *relocated_assigned_port_number);

    return 0;
  }

  if (result != 255) {
    printf("USB error %02x\r\n", result);
    return result;
  }

  return 255;
}
