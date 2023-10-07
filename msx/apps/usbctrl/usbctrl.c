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

    for (uint8_t i = 1; i <= l; i++) {
      hl = extbio_usb_get_device_descriptor(i, &my_device_descriptor);
      printf("Device %d: %02X\r\n", i, hl);
      logDevice(&my_device_descriptor);
    }
  }

  return 0;
}
