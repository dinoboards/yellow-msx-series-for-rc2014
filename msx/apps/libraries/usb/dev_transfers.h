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

  uint8_t  number : 3;
  uint16_t max_packet_sizex : 10;
} endpoint;

#define COMMON_DEVICE_CONFIG                                                                                                       \
  usb_device_type type : 4;                                                                                                        \
  uint8_t         address : 4;                                                                                                     \
  uint8_t         max_packet_size;                                                                                                 \
  uint8_t         interface_number;

typedef struct {
  COMMON_DEVICE_CONFIG
  endpoint_param endpoints[3]; // bulk in/out and interrupt
} device_config;

typedef struct {
  COMMON_DEVICE_CONFIG
} device_config_hub;

#define PRINTER_BUFFER_SIZE 64
typedef struct {
  COMMON_DEVICE_CONFIG
  endpoint_param endpoints[1]; // bulk out
  uint8_t        buffer_length;
  int8_t         buffer_wait;
  uint8_t        buffer[PRINTER_BUFFER_SIZE];
} device_config_printer;

typedef struct {
  COMMON_DEVICE_CONFIG
  endpoint_param endpoints[1]; // Isochronous
} device_config_keyboard;

typedef struct {
  COMMON_DEVICE_CONFIG
  endpoint_param endpoints[2]; // Bulk in and out
} device_config_ftdi;

extern usb_error usbdev_control_transfer(device_config *const device, const setup_packet *const cmd, uint8_t *const buffer);

extern usb_error usbdev_bulk_out_transfer(device_config *const device, const uint8_t *const buffer, const uint16_t buffer_size);

extern usb_error usbdev_data_in_transfer(device_config *const    device,
                                         uint8_t *const          buffer,
                                         const uint16_t          buffer_size,
                                         const usb_endpoint_type endpoint_type);

extern usb_error usbdev_data_in_transfer_ep0(device_config *const device, uint8_t *const buffer, const uint8_t buffer_size)
    __sdcccall(1);

#endif
