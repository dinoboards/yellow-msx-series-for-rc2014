#include <delay.h>
#include <extbio.h>
#include <extbio_rc2014.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "enumerate_trace.h"

uint8_t main(const int argc, const char *argv[]) {
  (void)argc;
  (void)argv;

  uint16_t      hl = extbio_usb_get_present();
  const uint8_t h  = hl >> 8;
  const uint8_t l  = hl & 0xFF;
  uint8_t       buffer[151];

  printf("CH376 present %s\r\n", h ? "yes" : "no");

  device_descriptor my_device_descriptor;
  memset(&my_device_descriptor, 0, sizeof(device_descriptor));

  printf("My address is %04X\r\n", &my_device_descriptor);

  if (h) {
    printf("Number of usb devices connected: %d\r\n", l);

    // find first floppy drive
    for (uint8_t device_address = 1; device_address <= l; device_address++) {
      usb_error result = usbtrn_get_descriptor2(&my_device_descriptor, device_address);
      if (result) {
        printf("usbtrn_get_descriptor2 failed %d\r\n", result);
        continue;
      }

      logDevice(&my_device_descriptor);
      getchar();

      for (uint8_t config_index = 0; config_index < my_device_descriptor.bNumConfigurations; config_index++) {
        memset(buffer, 0, sizeof(buffer));
        result = usbtrn_get_full_config_descriptor(config_index, device_address, my_device_descriptor.bMaxPacketSize0, 150, buffer);
        if (result) {
          printf("usbtrn_get_full_config_descriptor failed %d\r\n", result);
          continue;
        }

        const config_descriptor *const config = (config_descriptor *)buffer;
        logConfig(config);

        for (uint8_t interface_index = 0; interface_index < config->bNumInterfaces; interface_index++) {
          const interface_descriptor *const interface =
              (interface_descriptor *)(buffer + sizeof(config_descriptor) + interface_index * sizeof(interface_descriptor));
          logInterface(interface);

          // for(uint8_t endpoint_index = 0; endpoint_index < interface->bNumEndpoints; endpoint_index++) {
          //     const endpoint_descriptor *const endpoint = (endpoint_descriptor *)(buffer + sizeof(config_descriptor) +
          //     sizeof(interface_descriptor) + endpoint_index * sizeof(endpoint_descriptor)); logEndpoint(endpoint);
          // }
        }
      }
    }
  }

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
