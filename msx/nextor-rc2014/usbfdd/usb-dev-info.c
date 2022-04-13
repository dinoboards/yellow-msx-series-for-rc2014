#include "nextor.h"
#include "ufi.h"
#include "usb-dev-info-ufi.h"
#include "usb-dev.h"
#include "work-area.h"
#include <stdlib.h>
#include <string.h>

uint8_t usb_dev_info(const uint8_t device_index, const dev_info_request request_info, uint8_t *const buffer) {
  storage_device_config *const dev = get_usb_driver(device_index);

  switch (dev->type) {
  case USB_IS_FLOPPY:
    return usb_dev_info_ufi(dev, request_info, buffer);

  default:
    return 1;
  }
}
