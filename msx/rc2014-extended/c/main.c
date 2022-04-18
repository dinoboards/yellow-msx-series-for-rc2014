#include "ch376.h"
#include "print.h"

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
}