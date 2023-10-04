#include "usb-dev-write-ufi.h"
#include "nextor.h"
#include <class_ufi.h>
#include <delay.h>
#include <stdlib.h>
#include <string.h>

#include "enumerate_trace.h"

uint8_t usb_dev_write_ufi(device_config *const dev,
                          const uint8_t        lun,
                          uint8_t              number_sectors_to_write,
                          uint32_t             sector_number,
                          uint8_t *            buffer,
                          uint8_t *const       number_of_sectors_written) {
  usb_error result;
  if (lun != 1)
    return NEXTOR_ERR_IDEVL;

  ufi_interrupt_status sense_codes;

  printf("write sector %d\r\n", sector_number);

  *number_of_sectors_written = 0;

  if (wait_for_device_ready(dev, 2000) != 0)
    return NEXTOR_ERR_NRDY;

  printf("Ready - now requesting write\r\n");
  memset(&sense_codes, 0, sizeof(sense_codes));
  if ((result = ufi_read_write_sector(dev, true, sector_number, number_sectors_to_write, buffer, (uint8_t *)&sense_codes)) !=
      USB_ERR_OK) {
    printf("ufi_read_write_sector: %d (%d, %d)\r\n", result, sense_codes.bASC, sense_codes.bASCQ);
    return NEXTOR_ERR_WRERR;
    // goto retry;
  }

  ufi_request_sense_response response;
  memset(&response, 0, sizeof(response));

  printf("OK with sense_codes (%d, %d).  Now requesting full sense\r\n", sense_codes.bASC, sense_codes.bASCQ);

  if ((result = ufi_request_sense(dev, &response)) != USB_ERR_OK) {
    printf("ufi_request_sense: %d\r\n", result);
    return NEXTOR_ERR_WRERR;
    // goto retry;
  }

  const uint8_t asc       = response.asc;
  const uint8_t ascq      = response.ascq;
  const uint8_t sense_key = response.sense_key;

  printf("ufi_inquiry_response: %d, %d, %d\r\n", sense_key, asc, ascq);

  if (sense_key != 0)
    return NEXTOR_ERR_WRERR;

  *number_of_sectors_written = number_sectors_to_write;

  return NEXTOR_ERR_OK;
}
