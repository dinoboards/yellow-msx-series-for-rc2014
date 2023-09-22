#include <class_ufi.h>
#include "nextor.h"
#include "usb-dev-info-scsi.h"
#include "usb-dev-info-ufi.h"
#include "usb-dev.h"
#include "work-area.h"
#include <stdlib.h>
#include <string.h>

typedef uint8_t (*usb_dev_info_driver)(device_config *const dev, const dev_info_request request_info, uint8_t *const buffer);

static uint8_t no_driver(device_config *const dev, const dev_info_request request_info, uint8_t *const buffer) {
  (void)dev;
  (void)request_info;
  (void)buffer;
  return 1;
}

static const usb_dev_info_driver drivers[] = {&no_driver, &usb_dev_info_ufi, &usb_dev_info_scsi};

uint8_t usb_dev_info(const uint8_t device_index, const dev_info_request request_info, uint8_t *const buffer) {
  device_config *const  dev  = get_usb_driver(device_index);
  const usb_device_type type = dev->type;

  return drivers[type](dev, request_info, buffer);
}
