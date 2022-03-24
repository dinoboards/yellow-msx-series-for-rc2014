#include "nextor.h"
#include <stdlib.h>

uint8_t usb_dev_info(const dev_info_request request_info, uint8_t *const buffer) {
  (void)request_info;
  (void)buffer;

  return 1;
}