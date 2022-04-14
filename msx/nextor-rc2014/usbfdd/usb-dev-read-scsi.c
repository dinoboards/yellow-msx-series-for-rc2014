#include "usb-dev-read-scsi.h"
#include "nextor.h"
#include "scsi.h"

uint8_t usb_dev_read_scsi(storage_device_config *const dev,
                          const uint8_t                lun,
                          uint8_t                      number_sectors_to_read,
                          uint32_t                     sector_number,
                          uint8_t *                    buffer,
                          uint8_t *const               number_of_sectors_read) {

  if (lun != 1)
    return NEXTOR_ERR_IDEVL;

  while (number_sectors_to_read-- != 0) {
    if (scsi_read(dev, sector_number, buffer) != USB_ERR_OK)
      return NEXTOR_ERR_DISK;

    sector_number++;
    buffer += 512;
    (*number_of_sectors_read)++;
  }

  return NEXTOR_ERR_OK;
}