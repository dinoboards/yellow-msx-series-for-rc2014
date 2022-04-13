#include "usb-dev-info-scsi.h"
#include "nextor.h"
#include "ufi.h"
#include "usb-dev-info.h"
#include "work-area.h"
#include <stdlib.h>
#include <string.h>

uint8_t usb_dev_info_scsi(storage_device_config *const dev, const dev_info_request request_info, uint8_t *const buffer) {
  (void)dev;
  (void)request_info;
  (void)buffer;

  return 1;

  // if (request_info == BASIC_INFORMATION)
  //   return usb_dev_info_basic_information(buffer);

  // return 1;
}
