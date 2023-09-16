#ifndef __USB_ENUMERATE
#define __USB_ENUMERATE

#include "ch376.h"
#include "usb.h"
#include "work-area.h"
#include <stdbool.h>

#define MAX_CONFIG_SIZE 140

typedef struct __device_control {
  uint8_t index;

  usb_error (*reset_device)(const struct __device_control *const args) __z88dk_fastcall;
} _device_control;

struct __working;

typedef struct {
  // _device_control device_control;
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
