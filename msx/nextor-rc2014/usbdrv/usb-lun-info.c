#include "nextor.h"
#include "usb-dev.h"
#include "usb-lun-info-scsi.h"
#include "usb-lun-info-ufi.h"
#include "work-area.h"
#include <stdlib.h>

typedef uint8_t (*usb_lun_info_driver)(device_config *const dev, const uint8_t lun, nextor_lun_info *const info);

static uint8_t no_driver(device_config *const dev, const uint8_t lun, nextor_lun_info *const info) {
  (void)dev;
  (void)lun;
  (void)info;
  return 1;
}

static const usb_lun_info_driver drivers[] = {&no_driver, &usb_lun_info_ufi, &usb_lun_info_scsi};

uint8_t usb_lun_info(const uint8_t device_index, const uint8_t lun, nextor_lun_info *const info) {
  device_config *const  dev  = get_usb_driver(device_index);
  const usb_device_type type = dev->type;

  return drivers[type](dev, lun, info);
}
