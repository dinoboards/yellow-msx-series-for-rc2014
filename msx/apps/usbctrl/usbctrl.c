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

    hl = extbio_usb_get_device_descriptor(2, &my_device_descriptor);

    printf("Device %d: %04X\r\n", 1, hl);
    // printf("X %d\r\n", my_device_descriptor.bDescriptorType);
    logDevice(&my_device_descriptor);

    // for(uint8_t device_index = 1; device_index <= l; device_index++) {
    //   hl = extbio_usb_get_device_descriptor(device_index, &my_device_descriptor);
    //   printf("Device %d: %d\r\n", device_index, hl);
    // }
  }

  return 0;
}
