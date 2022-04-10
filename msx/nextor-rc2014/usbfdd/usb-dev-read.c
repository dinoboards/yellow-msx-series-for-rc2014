#include "nextor.h"
#include "ufi.h"
#include "work-area.h"
#include <stdlib.h>

uint8_t usb_dev_read(const uint8_t  device_index,
                     const uint8_t  lun,
                     uint8_t        number_sectors_to_read,
                     uint32_t       sector_number,
                     uint8_t *      buffer,
                     uint8_t *const number_of_sectors_read) {
  if (lun != 1)
    return NEXTOR_ERR_IDEVL;

  _usb_state *const work_area = get_usb_work_area();

  while (number_sectors_to_read-- != 0) {
    if (ufi_read_sector(&work_area->storage_device[device_index - 1], sector_number, buffer) != USB_ERR_OK)
      return NEXTOR_ERR_DISK;

    sector_number++;
    buffer += 512;
    (*number_of_sectors_read)++;
  }

  return NEXTOR_ERR_OK;
}