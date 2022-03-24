#include "nextor.h"
#include <stdlib.h>

uint8_t usb_dev_write(const uint8_t  lun,
                      const uint8_t  number_sectors_to_write,
                      uint32_t       sector_number,
                      uint8_t *      buffer,
                      uint8_t *const number_of_sectors_written) {

  (void)lun;
  (void)number_sectors_to_write;
  (void)sector_number;
  (void)buffer;
  (void)number_of_sectors_written;

  return NEXTOR_ERR_DISK;
}
