#include "command_report_all_devices.h"
#include "arguments.h"
#include "reporters.h"
#include "rotator.h"
#include <fusion_ext.h>
#include <io.h>
#include <protocol.h>
#include <string.h>

usb_error command_report_all_devices(const uint8_t last_device_address) __sdcccall(1) {
  uint8_t   buffer[151];
  usb_error result      = 0;
  int       file_handle = 0;

  printf("Enumerating %d devices\r\n", last_device_address);

  rotate_init();

  memset(buffer, 0, sizeof(buffer));
  file_handle = Open((char *)report_file_name, IO_CREAT);

  device_descriptor my_device_descriptor;
  memset(&my_device_descriptor, 0, sizeof(device_descriptor));

  fusion_fprintf(file_handle, "Device Count: %d\r\n", last_device_address);

  for (uint8_t device_address = 1; device_address <= last_device_address; device_address++) {
    erase_line();
    printf("\rdevice %d  ", device_address);
    rotate();
    usb_error result = usbtrn_get_descriptor2(&my_device_descriptor, device_address);
    if (result) {
      printf("usbtrn_get_descriptor2 failed %d\r\n", result);
      continue;
    }

    report_device_descriptor(&my_device_descriptor, file_handle);

    for (uint8_t config_index = 0; config_index < my_device_descriptor.bNumConfigurations; config_index++) {
      memset(buffer, 0, sizeof(buffer));
      result = usbtrn_get_full_config_descriptor(config_index, device_address, my_device_descriptor.bMaxPacketSize0, 150, buffer);
      if (result) {
        printf("usbtrn_get_full_config_descriptor failed %d\r\n", result);
        continue;
      }

      const config_descriptor *const config = (config_descriptor *)buffer;
      report_device_configuration(config, file_handle);

      for (uint8_t interface_index = 0; interface_index < config->bNumInterfaces; interface_index++) {
        const interface_descriptor *const interface =
            (interface_descriptor *)(buffer + sizeof(config_descriptor) + interface_index * sizeof(interface_descriptor));
        report_device_interface(interface, file_handle);

        for (uint8_t endpoint_index = 0; endpoint_index < interface->bNumEndpoints; endpoint_index++) {
          const endpoint_descriptor *const endpoint = (endpoint_descriptor *)&interface[1];
          report_device_endpoint(&endpoint[endpoint_index], file_handle);
        }
      }
    }
  }

  Close(file_handle);
  rotate_stop();
  erase_line();

  return 0;
}
