#include "ch376.h"
#include "print.h"
#include "usb-enumerate.h"
#include <delay.h>

usb_error usb_host_bus_reset() {
  ch_cmd_set_usb_mode(CH_MODE_HOST);
  delay_medium();

  ch_cmd_set_usb_mode(CH_MODE_HOST_RESET);
  delay_medium();

  ch_cmd_set_usb_mode(CH_MODE_HOST);
  delay_medium();

  ch_configure_nak_retry_3s();

  return USB_ERR_OK;
}

void usb_init() {
  ch_cmd_reset_all();

  if (!ch_probe()) {
    print_string("CH376:           NOT PRESENT\r\n");
    return;
  }

  const uint8_t ver = ch_cmd_get_ic_version();
  print_string("CH376:           PRESENT (VER ");
  print_hex(ver);
  print_string(")\r\n");

  print_string("USB:             SCANNING...");

  usb_host_bus_reset();

  enumerate_all_devices();
}