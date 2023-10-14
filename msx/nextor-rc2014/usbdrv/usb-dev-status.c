#include "nextor.h"
#include "usb-dev-status-scsi.h"
#include "usb-dev-status-ufi.h"
#include "usb-dev.h"
#include "work-area.h"
#include <class_ufi.h>
#include <stdlib.h>

typedef uint8_t (*usb_dev_status_driver)(device_config *const storage_device, const uint8_t lun);

static uint8_t no_driver(device_config *const storage_device, const uint8_t lun) {
  (void)storage_device;
  (void)lun;
  return DEV_STATUS_NOT_AVAILABLE_OR_INVALID;
}

static const usb_dev_status_driver drivers[] = {&no_driver, &usb_dev_status_ufi, &usb_dev_status_scsi};

uint8_t usb_dev_status(const uint8_t device_index, const uint8_t lun) {
  device_config *const  dev  = get_usb_device_config(device_index);
  const usb_device_type type = dev->type;

  return drivers[type](dev, lun);
}
