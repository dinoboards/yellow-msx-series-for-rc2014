#include "nextor.h"
#include "ufi.h"
#include "work-area.h"
#include <stdlib.h>

uint8_t usb_dev_write(const uint8_t  lun,
                      uint8_t        number_sectors_to_write,
                      uint32_t       sector_number,
                      uint8_t *      buffer,
                      uint8_t *const number_of_sectors_written) {
  if (lun != 1)
    return NEXTOR_ERR_IDEVL;

  _usb_state *const work_area = get_usb_work_area();

  while (number_sectors_to_write-- != 0) {
    if (ufi_write_sector(work_area, sector_number, buffer) != USB_ERR_OK)
      return NEXTOR_ERR_DISK;

    sector_number++;
    buffer += 512;
    (*number_of_sectors_written)++;
  }

  return NEXTOR_ERR_OK;
}
