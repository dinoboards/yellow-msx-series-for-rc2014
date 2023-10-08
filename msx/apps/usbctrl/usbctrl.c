#include "arguments.h"
#include "reporters.h"
#include "rotator.h"
#include <delay.h>
#include <extbio.h>
#include <extbio_rc2014.h>
#include <io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint8_t report_all_devices(const uint8_t last_device_address, const char *const filename) __sdcccall(1) {
  uint8_t   buffer[151];
  usb_error result      = 0;
  FILE     *file_handle = NULL;

  printf("Enumerating %d devices\r\n", last_device_address);

  rotate_init();

  memset(buffer, 0, sizeof(buffer));
  file_handle = fopen(filename, "w");

  device_descriptor my_device_descriptor;
  memset(&my_device_descriptor, 0, sizeof(device_descriptor));

  fprintf(file_handle, "Device Count: %d\r", last_device_address);

  for (uint8_t device_address = 1; device_address <= last_device_address; device_address++) {
    erase_line();
    printf("\rdevice %d  ", device_address);
    usb_error result = usbtrn_get_descriptor2(&my_device_descriptor, device_address);
    if (result) {
      printf("usbtrn_get_descriptor2 failed %d\r\n", result);
      continue;
    }

    report_device_descriptor(&my_device_descriptor, file_handle);

    for (uint8_t config_index = 0; config_index < my_device_descriptor.bNumConfigurations; config_index++) {
      erase_line();
      printf("\rdevice %d config %d  ", device_address, config_index);

      memset(buffer, 0, sizeof(buffer));
      result = usbtrn_get_full_config_descriptor(config_index, device_address, my_device_descriptor.bMaxPacketSize0, 150, buffer);
      if (result) {
        printf("usbtrn_get_full_config_descriptor failed %d\r\n", result);
        continue;
      }

      const config_descriptor *const config = (config_descriptor *)buffer;
      report_device_configuration(config, file_handle);

      for (uint8_t interface_index = 0; interface_index < config->bNumInterfaces; interface_index++) {
        erase_line();
        printf("\rdevice %d config %d interface %d ", device_address, config_index, interface_index);
        const interface_descriptor *const interface =
            (interface_descriptor *)(buffer + sizeof(config_descriptor) + interface_index * sizeof(interface_descriptor));
        report_device_interface(interface, file_handle);

        // for(uint8_t endpoint_index = 0; endpoint_index < interface->bNumEndpoints; endpoint_index++) {
        //     const endpoint_descriptor *const endpoint = (endpoint_descriptor *)(buffer + sizeof(config_descriptor) +
        //     sizeof(interface_descriptor) + endpoint_index * sizeof(endpoint_descriptor)); logEndpoint(endpoint);
        // }
      }
    }
  }

  fclose(file_handle);
  rotate_stop();
  erase_line();
}

uint8_t main(const int argc, const char *argv[]) {
  process_cli_arguments(argc, argv);

  uint16_t      hl = extbio_usb_get_present();
  const uint8_t h  = hl >> 8;
  const uint8_t l  = hl & 0xFF;

  printf("CH376 present %s\r\n", h ? "yes" : "no");

  if (!h)
    return 255;

  switch (subcommand) {
  case none:
    break;

  case report_to_file:
    report_all_devices(l, report_file_name);
    printf("usb device details saved to file %s\r\n", report_file_name);
    break;
  }

  // if (h) {
  //   report_all_devices(l);
  //   // find first floppy drive
  // }

  // ufi_format_capacities_response r;
  // memset(&r, 0, sizeof(r));

  // device_config storage_device;

  // if (wait_for_device_ready(&storage_device, 2500) == 0) {
  //   hl = ufi_read_format_capacities(&storage_device, &r);
  //   printf("ufi_read_format_capacities: %d,", hl);
  //   log_ufi_format_capacities_response(&r);
  // }

  return 0;
}
