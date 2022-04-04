#include "ch376.h"
#include "hw.h"
#include "nextor.h"
#include "usb-enumerate-hub.h"
#include "usb-enumerate.h"
#include "usb.h"
#include "work-area.h"
#include <delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "debuggin.h"
#include "print.h"

usb_error read_all_configs(_usb_state *const work_area);

usb_error usb_host_bus_reset() {
  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60 / 4);

  ch376_set_usb_mode(CH_MODE_HOST_RESET);
  delay(60 / 2);

  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60 / 4);

  ch_configure_nak_retry_indefinite();

  return USB_ERR_OK;
}

void state_devices(const _usb_state *const work_area) {
  if (work_area->usb_device == 0)
    return;

  print_string("USB:             ");

  usb_device_type dev = work_area->usb_device;

  if (dev & USB_IS_HUB) {
    print_string("HUB");

    dev &= ~USB_IS_HUB;
    if (dev)
      print_string(", ");
  }

  if (dev & USB_IS_FLOPPY) {
    print_string("FLOPPY");

    dev &= ~USB_IS_FLOPPY;
    if (dev)
      print_string(", ");
  }

  if (dev & USB_IS_MASS_STORAGE)
    print_string("STORAGE");

  print_string("\r\n");
}

uint8_t usb_host_init() {
  work_area *const p = get_work_area();
  __asm__("EI");
  _usb_state *const work_area = &p->ch376;
  memset(work_area, 0, sizeof(_usb_state));

  ch376_reset();

  if (!ch376_probe()) {
    print_string("CH376:           NOT PRESENT\r\n");
    return false;
  }

  const uint8_t ver = ch376_get_firmware_version();
  print_string("CH376:           PRESENT (VER ");
  print_hex(ver);
  print_string(")\r\n");

  usb_host_bus_reset();
  delay(10);

  read_all_configs(work_area);

  state_devices(work_area);

  return true;
}
