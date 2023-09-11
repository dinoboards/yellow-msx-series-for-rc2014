#ifndef __USB_DEV_INFO
#define __USB_DEV_INFO

#include "nextor.h"

extern uint8_t usb_dev_info(const uint8_t device_index, const dev_info_request request_info, uint8_t *const buffer);

inline uint8_t usb_dev_info_basic_information(uint8_t *buffer) {
  *buffer++ = 1;
  *buffer = 0;
  return 0;
}

#endif
