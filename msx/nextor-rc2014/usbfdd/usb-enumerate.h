#ifndef __USB_ENUMERATE
#define __USB_ENUMERATE

#include "usb.h"
#include "work-area.h"

#define MAX_CONFIG_SIZE 140

struct __working;

typedef struct {
  uint8_t next_storage_device_index;
  uint8_t next_device_address;
} enumeration_state;

typedef struct __working {
  enumeration_state *state;

  usb_device_type   usb_device;
  device_descriptor desc;
  uint8_t           config_index;
  uint8_t           interface_count;
  uint8_t           endpoint_count;

  const uint8_t *ptr;

  union {
    uint8_t           buffer[MAX_CONFIG_SIZE];
    config_descriptor desc;
  } config;
} _working;

extern usb_error read_all_configs(enumeration_state *const state);
extern usb_error enumerate_all_devices();

#endif
