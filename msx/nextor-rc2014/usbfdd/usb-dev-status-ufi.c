#include "nextor.h"
#include "ufi.h"
#include "work-area.h"
#include <stdlib.h>

uint8_t usb_dev_status_ufi(storage_device_config *const storage_device, const uint8_t lun) {

  if (lun != 1)
    return DEV_STATUS_NOT_AVAILABLE_OR_INVALID;

  const usb_error result = test_disk(storage_device);

  if (result == USB_ERR_MEDIA_NOT_PRESENT)
    return DEV_STATUS_NOT_AVAILABLE_OR_INVALID;

  if (result == USB_ERR_MEDIA_CHANGED)
    return DEV_STATUS_AVAILABLE_AND_CHANGED;

  return DEV_STATUS_AVAILABLE_AND_NOT_CHANGED;
}
