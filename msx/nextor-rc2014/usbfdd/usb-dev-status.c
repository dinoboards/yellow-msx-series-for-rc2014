#include "nextor.h"
#include "ufi.h"
#include "work-area.h"
#include <stdlib.h>

uint8_t usb_dev_status(const uint8_t device_index, const uint8_t lun) {
  if (lun != 1 || device_index != 1)
    return DEV_STATUS_NOT_AVAILABLE_OR_INVALID;

  _usb_state *const work_area = get_usb_work_area();

  if (!(work_area->usb_device & USB_IS_FLOPPY))
    return DEV_STATUS_NOT_AVAILABLE_OR_INVALID;

  const usb_error result = test_disk(work_area);

  if (result == USB_ERR_MEDIA_NOT_PRESENT)
    return DEV_STATUS_NOT_AVAILABLE_OR_INVALID;

  if (result == USB_ERR_MEDIA_CHANGED)
    return DEV_STATUS_AVAILABLE_AND_CHANGED;

  return DEV_STATUS_AVAILABLE_AND_NOT_CHANGED;
}
