#include "nextor.h"
#include "usb-dev.h"
#include "usb-lun-info-scsi.h"
#include "usb-lun-info-ufi.h"
#include "work-area.h"
#include <stdlib.h>

uint8_t usb_lun_info(const uint8_t device_index, const uint8_t lun, nextor_lun_info *const info) {
  storage_device_config *const dev = get_usb_driver(device_index);

  switch (dev->type) {
  case USB_IS_FLOPPY:
    return usb_lun_info_ufi(dev, lun, info);

  case USB_IS_MASS_STORAGE:
    return usb_lun_info_scsi(dev, lun, info);

  default:
    return 1;
  }
}