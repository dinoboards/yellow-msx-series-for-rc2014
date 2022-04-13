#ifndef __USB_DEV_INFO
#define __USB_DEV_INFO

inline uint8_t usb_dev_info_basic_information(uint8_t *buffer) {
  *buffer++ = 1;
  *buffer   = 0;
  return 0;
}

#endif
