#ifndef DEVICE_SEARCH_H
#define DEVICE_SEARCH_H

#include "arguments.h"
#include <ch376.h>
#include <class_ufi.h>
#include <dev_transfers.h>
#include <stdbool.h>
#include <stdint.h>

usb_error get_device_config_from_drive_letter(device_config *storage_device);
usb_error find_next_floppy_interface(const uint8_t address, device_config_interface *const floppy_interface) __sdcccall(1);
usb_error retrieve_floppy_formats(device_config *const storage_device, ufi_format_capacities_response *const r) __sdcccall(1);
// =======
// typedef bool (*interface_match_fn)(const device_descriptor *const, const interface_descriptor *const) __sdcccall(1);

// extern usb_error get_device_config_from_drive_letter(device_config *storage_device);
// extern uint8_t   construct_device_config(const uint8_t        device_address,
//                                          const uint8_t        config_index,
//                                          const uint8_t        interface_index,
//                                          device_config *const storage_device) __sdcccall(1);
// extern usb_error find_next_floppy_interface(const uint8_t address, device_config_interface *const floppy_interface)
// __sdcccall(1); extern usb_error retrieve_floppy_formats(device_config *const storage_device, ufi_format_capacities_response
// *const r)
//     __sdcccall(1);
// extern usb_error find_device(const uint8_t address, interface_match_fn fn, device_config *const config) __sdcccall(1);
// >>>>>>> 8f0f7e6 (usbdrv: ftdi implemented initial read/write data)

#endif
