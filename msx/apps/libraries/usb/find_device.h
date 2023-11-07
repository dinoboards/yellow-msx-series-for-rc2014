#ifndef __USB_FIND_DEVICE_H
#define __USB_FIND_DEVICE_H

#include "ch376.h"
#include "protocol.h"
#include <stdbool.h>
#include <stdint.h>

typedef bool (*interface_match_fn)(const device_descriptor *const, const interface_descriptor *const) __sdcccall(1);

extern usb_error find_device(interface_match_fn fn, device_config *const config) __sdcccall(1);
extern uint8_t   construct_device_config(const uint8_t        device_address,
                                         const uint8_t        config_index,
                                         const uint8_t        interface_index,
                                         device_config *const storage_device) __sdcccall(1);
#endif
