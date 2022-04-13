#include "nextor.h"
#include "ufi.h"
#include "usb-dev-info-scsi.h"
#include "usb-dev-info-ufi.h"
#include "usb-dev.h"
#include "work-area.h"
#include <stdlib.h>
#include <string.h>

typedef uint8_t (*usb_dev_info_driver)(storage_device_config *const dev,
                                       const dev_info_request       request_info,
                                       uint8_t *const               buffer);

// const usb_dev_info_driver drivers[] = {
//   &usb_dev_info_ufi, &usb_dev_info_scsi
// };

uint8_t usb_dev_info(const uint8_t device_index, const dev_info_request request_info, uint8_t *const buffer) {
  storage_device_config *const dev = get_usb_driver(device_index);

  usb_dev_info_driver t;

  switch (dev->type) {
  case USB_IS_FLOPPY:
    t = &usb_dev_info_ufi;
    break;

  case USB_IS_MASS_STORAGE:
    t = &usb_dev_info_scsi;
    break;

  default:
    return 1;
  }

  return t(dev, request_info, buffer);
}
