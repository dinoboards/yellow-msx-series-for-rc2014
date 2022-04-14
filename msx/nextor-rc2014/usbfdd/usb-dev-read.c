#include "nextor.h"
#include "usb-dev-read-scsi.h"
#include "usb-dev-read-ufi.h"
#include "usb-dev.h"
#include "work-area.h"
#include <stdlib.h>

typedef uint8_t (*usb_dev_read_driver)(storage_device_config *const dev,
                                       const uint8_t                lun,
                                       uint8_t                      number_sectors_to_read,
                                       uint32_t                     sector_number,
                                       uint8_t *                    buffer,
                                       uint8_t *const               number_of_sectors_read);

static uint8_t no_driver(storage_device_config *const dev,
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

static const usb_dev_read_driver drivers[] = {&no_driver, &usb_dev_read_ufi, &usb_dev_read_scsi};

uint8_t usb_dev_read(const uint8_t  device_index,
                     const uint8_t  lun,
                     uint8_t        number_sectors_to_read,
                     uint32_t       sector_number,
                     uint8_t *      buffer,
                     uint8_t *const number_of_sectors_read) {

  storage_device_config *const dev  = get_usb_driver(device_index);
  const usb_device_type        type = dev->type;

  return drivers[type](dev, lun, number_sectors_to_read, sector_number, buffer, number_of_sectors_read);
}