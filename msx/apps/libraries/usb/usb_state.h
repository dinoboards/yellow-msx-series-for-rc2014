#ifndef __USB_STATE
#define __USB_STATE

#include "ch376.h"
#include "protocol.h"
#include <stdlib.h>

#define MAX_NUMBER_OF_STORAGE_DEVICES 4

typedef struct __usb_state {
  uint8_t               printer_time_throttle_flag : 1;
  uint8_t               count_of_detected_usb_devices : 4;
  uint8_t               version : 7;
  uint8_t               connected : 1;
  device_config         storage_device[MAX_NUMBER_OF_STORAGE_DEVICES];
  device_config         cdc_config;
  device_config_printer printer_config;
} _usb_state;

extern _usb_state *get_usb_work_area(void);

#endif
