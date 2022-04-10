#include "nextor.h"
#include "ufi.h"
#include "work-area.h"
#include <stdlib.h>
#include <string.h>

inline uint8_t usb_dev_info_basic_information(uint8_t *buffer) {
  *buffer++ = 1;
  *buffer   = 0;
  return 0;
}

uint8_t usb_dev_info(const uint8_t device_index, const dev_info_request request_info, uint8_t *const buffer) {
  if (device_index != 1)
    return 1;

  _usb_state *const work_area = get_usb_work_area();

  if (work_area->storage_device[0].type != USB_IS_FLOPPY)
    return 1;

  if (request_info == BASIC_INFORMATION)
    return usb_dev_info_basic_information(buffer);

  ufi_inquiry_response response;
  memset(&response, 0, sizeof(ufi_inquiry_response));
  const usb_error result = ufi_inquiry(&work_area->storage_device[0], &response);

  if (result != USB_ERR_OK)
    return 2;

  memset(buffer, ' ', 64);

  switch (request_info) {
  case MANUFACTURER_NAME:
    memcpy(buffer, response.vendor_information, 8);
    return 0;

  case DEVICE_NAME:
    memcpy(buffer, response.product_id, 16);
    return 0;

  case SERIAL_NUMBER:
    memcpy(buffer, response.product_revision, 4);
    return 0;

  default:
    return 1;
  }
}