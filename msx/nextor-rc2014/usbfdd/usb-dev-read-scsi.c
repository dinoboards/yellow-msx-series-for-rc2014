#include "usb-dev-read-scsi.h"
#include "nextor.h"
#include "scsi.h"

uint8_t usb_dev_read_scsi(storage_device_config *const dev,
                          const uint8_t                lun,
                          uint8_t                      number_sectors_to_read,
                          uint32_t                     sector_number,
                          uint8_t *                    buffer,
                          uint8_t *const               number_of_sectors_read) {
  (void)dev;
  (void)lun;
  (void)number_sectors_to_read;
  (void)sector_number;
  (void)buffer;
  (void)number_of_sectors_read;

  return NEXTOR_ERR_IDEVL;
}