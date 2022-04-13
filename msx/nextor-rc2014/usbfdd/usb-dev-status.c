#include "nextor.h"
#include "ufi.h"
#include "usb-dev-status-ufi.h"
#include "usb-dev.h"
#include "work-area.h"
#include <stdlib.h>

uint8_t usb_dev_status(const uint8_t device_index, const uint8_t lun) {
  storage_device_config *const dev = get_usb_driver(device_index);

  switch (dev->type) {
  case USB_IS_FLOPPY:
    return usb_dev_status_ufi(dev, lun);

  default:
    return DEV_STATUS_NOT_AVAILABLE_OR_INVALID;
  }
}
