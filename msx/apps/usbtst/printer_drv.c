#include "class_printer.h"
#include "usb_state.h"
#include <string.h>

uint8_t prt_send_text(const char *str) __sdcccall(1) {
  device_config *const printer_config = find_device_config(USB_IS_PRINTER);

  usb_error result = prt_send((device_config_printer *)printer_config, (uint8_t *)str, strlen(str));

  return result;
}
