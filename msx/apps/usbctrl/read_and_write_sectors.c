#include "read_and_write_sectors.h"
#include <class_ufi.h>
#include <string.h>

usb_error write_sector(device_config *const storage_device, const uint16_t sector_number, uint8_t *buffer) {
  usb_error            result;
  ufi_interrupt_status sense_codes;

  if (wait_for_device_ready(storage_device, 2000) != 0)
    return 255;

  memset(&sense_codes, 0, sizeof(sense_codes));
  if ((result = ufi_read_write_sector(storage_device, true, sector_number, 1, buffer, (uint8_t *)&sense_codes)) != USB_ERR_OK)
    return result;

  ufi_request_sense_response response;
  memset(&response, 0, sizeof(response));

  if ((result = ufi_request_sense(storage_device, &response)) != USB_ERR_OK)
    return result;

  const uint8_t asc       = response.asc;
  const uint8_t ascq      = response.ascq;
  const uint8_t sense_key = response.sense_key;

  if (sense_key != 0)
    return 255;

  return 0;
}

usb_error read_sector(device_config *const storage_device, const uint16_t sector_number, uint8_t *buffer) {

  if (wait_for_device_ready(storage_device, 2000) != 0)
    return 255;

  usb_error            result;
  ufi_interrupt_status sense_codes;

  memset(&sense_codes, 0, sizeof(sense_codes));

  if ((result = ufi_read_write_sector(storage_device, false, sector_number, 1, buffer, (uint8_t *)&sense_codes)) != USB_ERR_OK)
    return result;

  ufi_request_sense_response response;
  memset(&response, 0, sizeof(response));

  if ((result = ufi_request_sense(storage_device, &response)) != USB_ERR_OK)
    return result;

  const uint8_t asc       = response.asc;
  const uint8_t ascq      = response.ascq;
  const uint8_t sense_key = response.sense_key;

  if (sense_key != 0)
    return 255;

  return 0;
}
