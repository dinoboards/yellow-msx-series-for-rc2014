#include "work-area.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum _dev_info_request {
  BASIC_INFORMATION = 0,
  MANUFACTURER_NAME = 1,
  DEVICE_NAME       = 2,
  SERIAL_NUMBER     = 3
} dev_info_request;

void usb_dev_info_basic_information(uint8_t *buffer) __z88dk_fastcall {
  *buffer++ = 1;
  *buffer   = 0;
}

void usb_dev_info_manufacturer_name(uint8_t *const buffer) __z88dk_fastcall {
  const work_area *const p = get_work_area();

  memcpy(buffer, p->ch376.scsi_device_info.vendorid, 8);
  buffer[8] = 0;
}

void usb_dev_info_device_name(uint8_t *const buffer) __z88dk_fastcall {
  const work_area *const p = get_work_area();

  memcpy(buffer, p->ch376.scsi_device_info.productid, 16);
  buffer[16] = 0;
}

void usb_dev_info_serial_number(uint8_t *const buffer) __z88dk_fastcall {
  const work_area *const p = get_work_area();

  memcpy(buffer, p->ch376.scsi_device_info.productrev, 4);
  buffer[4] = 0;
}

uint8_t usb_dev_info(const dev_info_request request_info, uint8_t *const buffer) {
  switch (request_info) {
  case BASIC_INFORMATION:
    usb_dev_info_basic_information(buffer);
    return 0;

  case MANUFACTURER_NAME:
    usb_dev_info_manufacturer_name(buffer);
    return 0;

  case DEVICE_NAME:
    usb_dev_info_device_name(buffer);
    return 0;

  case SERIAL_NUMBER:
    usb_dev_info_serial_number(buffer);
    return 0;

  default:
    return 1;
  }
}
