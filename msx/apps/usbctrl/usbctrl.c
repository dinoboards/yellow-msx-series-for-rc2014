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

  printf("CH376 present %s\r\n", h ? "yes" : "no");

  device_descriptor my_device_descriptor;
  memset(&my_device_descriptor, 0, sizeof(device_descriptor));

  printf("My address is %04X\r\n", &my_device_descriptor);

  if (h) {
    printf("Number of usb devices connected: %d\r\n", l);

    // find first floppy drive
    for (uint8_t i = 1; i <= l; i++) {
      hl = usbtrn_get_descriptor2(&my_device_descriptor, i);
      printf("Device %d: %02X\r\n", i, hl);
      logDevice(&my_device_descriptor);
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
