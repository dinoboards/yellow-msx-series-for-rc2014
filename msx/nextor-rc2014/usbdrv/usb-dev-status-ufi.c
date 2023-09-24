#include "nextor.h"
#include "work-area.h"
#include <class_ufi.h>
#include <stdlib.h>
#include <string.h>

uint8_t usb_dev_status_ufi(device_config *const storage_device, const uint8_t lun) {
  usb_error result;

  if (lun != 1)
    return DEV_STATUS_NOT_AVAILABLE_OR_INVALID;

  if (wait_for_device_ready(storage_device, 50) != 0)
    return 1;

  ufi_request_sense_response response1;
  memset(&response1, 0, sizeof(ufi_request_sense_response));
  // ufi_request_sense_response response2;

  // ufi_test_unit_ready(storage_device, &response1);
  result = ufi_test_unit_ready(storage_device, &response1);

  CHECK(result);

  if (response1.sense_key != 0)
    return DEV_STATUS_NOT_AVAILABLE_OR_INVALID;

  // if (response1.sense_key == 2 || response1.sense_key == 6)
  //   return DEV_STATUS_AVAILABLE_AND_CHANGED;

  return DEV_STATUS_AVAILABLE;
}
