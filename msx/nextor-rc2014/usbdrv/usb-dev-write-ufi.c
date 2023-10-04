#include "usb-dev-write-ufi.h"
#include "nextor.h"
#include "work-area.h"
#include <class_ufi.h>
#include <stdlib.h>
#include <string.h>

uint8_t usb_dev_write_ufi(device_config *const dev,
                          const uint8_t        lun,
                          uint8_t              number_sectors_to_write,
                          uint32_t             sector_number,
                          uint8_t *            buffer,
                          uint8_t *const       number_of_sectors_written) {
  if (lun != 1)
    return NEXTOR_ERR_IDEVL;

  usb_error            result;
  ufi_interrupt_status sense_codes;

  *number_of_sectors_written = 0;

  if (wait_for_device_ready(dev, 2000) != 0)
    return NEXTOR_ERR_NRDY;

  memset(&sense_codes, 0, sizeof(sense_codes));
  if ((result = ufi_read_write_sector(dev, true, sector_number, number_sectors_to_write, buffer, (uint8_t *)&sense_codes)) !=
      USB_ERR_OK)
    return NEXTOR_ERR_WRERR;

  ufi_request_sense_response response;
  memset(&response, 0, sizeof(response));

  if ((result = ufi_request_sense(dev, &response)) != USB_ERR_OK)
    return NEXTOR_ERR_WRERR;

  const uint8_t asc       = response.asc;
  const uint8_t ascq      = response.ascq;
  const uint8_t sense_key = response.sense_key;

  if (sense_key != 0)
    return NEXTOR_ERR_WRERR;

  *number_of_sectors_written = number_sectors_to_write;

  return NEXTOR_ERR_OK;
}
