#include "usb-dev-info-ufi.h"
#include "nextor.h"
#include "ufi.h"
#include "usb-dev-info.h"
#include "work-area.h"
#include <stdlib.h>
#include <string.h>

uint8_t usb_dev_info_ufi(storage_device_config *const dev, const dev_info_request request_info, uint8_t *const buffer) {
  ufi_inquiry_response response;

  if (request_info == BASIC_INFORMATION)
    return usb_dev_info_basic_information(buffer);

  memset(&response, 0, sizeof(ufi_inquiry_response));
  const usb_error result = ufi_inquiry(dev, &response);

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