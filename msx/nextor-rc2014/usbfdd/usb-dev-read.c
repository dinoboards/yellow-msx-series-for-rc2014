#include "nextor.h"
#include <stdlib.h>

uint8_t usb_dev_read(const uint8_t lun, const uint8_t number_sectors_to_read, uint32_t sector_number, uint8_t *buffer, uint8_t *const number_of_sectors_read) {

  (void)lun;
  (void)number_sectors_to_read;
  (void)sector_number;
  (void)buffer;
  (void)number_of_sectors_read;

  return NEXTOR_ERR_DISK;
}