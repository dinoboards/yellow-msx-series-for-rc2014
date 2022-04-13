#include "nextor.h"
#include "ufi.h"
#include "work-area.h"
#include <stdlib.h>

uint8_t usb_dev_status_scsi(storage_device_config *const storage_device, const uint8_t lun) {
  (void)storage_device;
  (void)lun;

  return DEV_STATUS_NOT_AVAILABLE_OR_INVALID;
}
