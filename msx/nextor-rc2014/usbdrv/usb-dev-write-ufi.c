#include "usb-dev-write-ufi.h"
#include "nextor.h"
#include "work-area.h"
#include <class_ufi.h>
#include <stdlib.h>

uint8_t usb_dev_write_ufi(device_config *const dev,
                          const uint8_t        lun,
                          uint8_t              number_sectors_to_write,
                          uint32_t             sector_number,
                          uint8_t *            buffer,
                          uint8_t *const       number_of_sectors_written) {
  if (lun != 1)
    return NEXTOR_ERR_IDEVL;

  if (wait_for_device_ready(dev, 50) != 0)
    return NEXTOR_ERR_NRDY;

  if (ufi_read_write_sector(dev, true, sector_number, number_sectors_to_write, buffer) != USB_ERR_OK)
    return NEXTOR_ERR_DISK;

  *number_of_sectors_written = number_sectors_to_write;

  return NEXTOR_ERR_OK;
}
