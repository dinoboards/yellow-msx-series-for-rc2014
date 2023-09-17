#include "class-scsi.h"
#include "nextor.h"
#include "work-area.h"
#include <stdlib.h>

uint8_t usb_dev_status_scsi(device_config *const storage_device, const uint8_t lun) {
  scsi_sense_result response;

  if (lun != 1)
    return DEV_STATUS_NOT_AVAILABLE_OR_INVALID;

  usb_error result = scsi_test(storage_device);

  if (result == USB_ERR_OK)
    return DEV_STATUS_AVAILABLE_AND_NOT_CHANGED;

  result = scsi_request_sense(storage_device, &response);

  result = scsi_test(storage_device);

  if (result == USB_ERR_OK)
    return DEV_STATUS_AVAILABLE_AND_CHANGED;

  return DEV_STATUS_NOT_AVAILABLE_OR_INVALID;
}
