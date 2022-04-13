#include "ufi.h"
#include "usb-lun-info-ufi.h"
#include <stdlib.h>

uint8_t usb_lun_info_scsi(storage_device_config *const dev, const uint8_t lun, nextor_lun_info *const info) {
  (void)dev;
  (void)lun;
  (void)info;
  return 1;
}