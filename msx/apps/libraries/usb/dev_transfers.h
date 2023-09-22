/**
 * @file transfer.h
 * @author Dean Netherton
 * @brief A simplest implementation of common usb transfer functions, based on the CH376S chip
 * @details For a basic walkthrough of the usb protocol see https://www.beyondlogic.org/usbnutshell/usb1.shtml
 * @version 1.0
 * @date 2023-09-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __USBDEV_TRANSFERS
#define __USBDEV_TRANSFERS

#include "ch376.h"
#include "transfers.h"
#include <stdlib.h>

typedef struct {
  usb_device_type type;
  uint8_t         max_packet_size;
  uint8_t         interface_number;
  uint8_t         tag;
  uint8_t         address;
  endpoint_param  endpoints[3]; // bulk in/out and interrupt
} device_config;

extern usb_error usbdev_control_transfer(device_config *const device, const setup_packet *const cmd, uint8_t *const buffer);

extern usb_error usbdev_bulk_out_transfer(device_config *const device, const uint8_t *const buffer, const uint16_t buffer_size);

extern usb_error usbdev_data_in_transfer(device_config *const    device,
                                         uint8_t *const          buffer,
                                         const uint16_t          buffer_size,
                                         const usb_endpoint_type endpoint_type);
#endif
