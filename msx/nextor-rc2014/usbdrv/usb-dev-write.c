#include "nextor.h"
#include "usb-dev-write-scsi.h"
#include "usb-dev-write-ufi.h"
#include "usb-dev.h"
#include "work-area.h"
#include <stdlib.h>

typedef uint8_t (*usb_dev_write_driver)(device_config *const dev,
                                        const uint8_t        lun,
                                        uint8_t              number_sectors_to_write,
                                        uint32_t             sector_number,
                                        uint8_t             *buffer,
                                        uint8_t *const       number_of_sectors_written);

static uint8_t no_driver(device_config *const dev,
                         const uint8_t        lun,
                         uint8_t              number_sectors_to_write,
                         uint32_t             sector_number,
                         uint8_t             *buffer,
                         uint8_t *const       number_of_sectors_written) {

  (void)dev;
  (void)lun;
  (void)number_sectors_to_write;
  (void)sector_number;
  (void)buffer;
  (void)number_of_sectors_written;

  return NEXTOR_ERR_IDEVL;
}

static const usb_dev_write_driver drivers[] = {&no_driver, &usb_dev_write_ufi, &usb_dev_write_scsi};

uint8_t usb_dev_write(const uint8_t  device_index,
                      const uint8_t  lun,
                      uint8_t        number_sectors_to_write,
                      uint32_t       sector_number,
                      uint8_t       *buffer,
                      uint8_t *const number_of_sectors_written) {

  device_config *const  dev  = get_usb_device_config(device_index);
  const usb_device_type type = dev->type;

  return drivers[type](dev, lun, number_sectors_to_write, sector_number, buffer, number_of_sectors_written);
}
