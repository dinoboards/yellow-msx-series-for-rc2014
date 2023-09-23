#ifndef __USB_STATE
#define __USB_STATE

#include "ch376.h"
#include "protocol.h"
#include <stdlib.h>

#define MAX_NUMBER_OF_STORAGE_DEVICES 4

typedef struct __usb_state {
  device_config         storage_device[MAX_NUMBER_OF_STORAGE_DEVICES];
  device_config_hub     hub_config;
  device_config         cdc_config;
  device_config_printer printer_config;
  uint8_t               printer_time_throttle_flag;
} _usb_state;

extern _usb_state *get_usb_work_area();

#endif
