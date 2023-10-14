#ifndef DEVICE_SEARCH_H
#define DEVICE_SEARCH_H

#include "arguments.h"
#include <ch376.h>
#include <class_ufi.h>
#include <dev_transfers.h>
#include <stdbool.h>
#include <stdint.h>

usb_error find_next_floppy_interface(const uint8_t address, device_config_interface *const floppy_interface) __sdcccall(1);
usb_error construct_device_config(device_config *const storage_device) __sdcccall(1);
usb_error retrieve_floppy_formats(device_config *const storage_device, ufi_format_capacities_response *const r) __sdcccall(1);

#endif
