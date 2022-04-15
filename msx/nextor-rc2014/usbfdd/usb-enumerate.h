#ifndef __USB_ENUMERATE
#define __USB_ENUMERATE

#include "usb.h"
#include "work-area.h"

struct __working;

typedef struct __working {
  uint8_t next_storage_device_index;
  uint8_t next_device_address;

  usb_device_type   usb_device;
  device_descriptor desc;
  uint8_t           config_index;
  uint8_t           interface_count;
  uint8_t           endpoint_count;

  const void *ptr;

  union {
    uint8_t           buffer[140];
    config_descriptor desc;
  } config;
} _working;

extern usb_error read_all_configs(uint8_t *const next_storage_device_index);
extern usb_error enumerate_all_devices();

#endif
