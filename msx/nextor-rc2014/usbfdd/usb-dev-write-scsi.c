#include "usb-dev-write-scsi.h"
#include "nextor.h"
#include <class_scsi.h>

uint8_t usb_dev_write_scsi(device_config *const dev,
                           const uint8_t        lun,
                           const uint8_t        number_sectors_to_write,
                           const uint32_t       sector_number,
                           const uint8_t *const buffer,
                           uint8_t *const       number_of_sectors_written) {
  if (lun != 1)
    return NEXTOR_ERR_IDEVL;

  if (scsi_read_write(dev, true, sector_number, number_sectors_to_write, (uint8_t *)buffer) != USB_ERR_OK)
    return NEXTOR_ERR_DISK;

  *number_of_sectors_written = number_sectors_to_write;

  return NEXTOR_ERR_OK;
}
