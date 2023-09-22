/**
 * @file protocol.c
 * @author Dean Netherton
 * @brief A simplest implementation of common usb transfer functions, based on the CH376S chip
 * @details For a basic walkthrough of the usb protocol see https://www.beyondlogic.org/usbnutshell/usb1.shtml
 * @version 1.0
 * @date 2023-09-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "protocol.h"
#include "ch376.h"
#include <delay.h>
#include <stdlib.h>

const setup_packet cmd_get_device_descriptor = {0x80, 6, {0, 1}, {0, 0}, 8};

/**
 * @brief Get the device descriptor for usb device at address 0
 *
 * @param buffer the buffer to store the device descriptor in
 * @return usb_error USB_ERR_OK if all good, otherwise specific error code
 */
usb_error hw_get_description(device_descriptor *const buffer) {
  usb_error    result;
  setup_packet cmd;
  cmd         = cmd_get_device_descriptor;
  cmd.wLength = 8;

  result = hw_control_transfer(&cmd, (uint8_t *)buffer, 0, 8);

  CHECK(result);

  cmd         = cmd_get_device_descriptor;
  cmd.wLength = 18;
  CHECK(hw_control_transfer(&cmd, (uint8_t *)buffer, 0, buffer->bMaxPacketSize0));

  return result;
}

const setup_packet cmd_set_device_address = {0x00, 5, {0, 0}, {0, 0}, 0};

/**
 * @brief configure device at address 0 to be assigned a new device address
 *
 * @param device_address the new device address
 * @return usb_error USB_ERR_OK if all good, otherwise specific error code
 */
usb_error hw_set_address(const uint8_t device_address) __z88dk_fastcall {
  setup_packet cmd;
  cmd           = cmd_set_device_address;
  cmd.bValue[0] = device_address;

  return hw_control_transfer(&cmd, 0, 0, 0);
}

const setup_packet cmd_set_configuration = {0x00, 9, {0, 0}, {0, 0}, 0};

/**
 * @brief
 *
 * @param config
 * @return usb_error
 */
usb_error hw_set_configuration(const device_config *const config) __z88dk_fastcall {
  setup_packet cmd;
  cmd           = cmd_set_configuration;
  cmd.bValue[0] = config->value;

  return hw_control_transfer(&cmd, 0, config->address, config->max_packet_size);
}

const setup_packet cmd_get_config_descriptor = {0x80, 6, {0, 2}, {0, 0}, 0};

usb_error hw_get_config_descriptor(config_descriptor *const buffer,
                                   const uint8_t            config_index,
                                   const uint8_t            buffer_size,
                                   const uint8_t            device_address,
                                   const uint8_t            max_packet_size) {
  setup_packet cmd;
  cmd           = cmd_get_config_descriptor;
  cmd.bValue[0] = config_index;
  cmd.wLength   = (uint16_t)buffer_size;

  return hw_control_transfer(&cmd, (uint8_t *)buffer, device_address, max_packet_size);
}

setup_packet usb_cmd_clear_endpoint_halt = {2, 1, {0, 0}, {255, 0}, 0}; //    ;byte 4 is the endpoint to be cleared

usb_error usb_clear_endpoint_halt(device_config *const storage_device, const usb_endpoint_type endpoint_type) {
  setup_packet cmd;
  cmd           = usb_cmd_clear_endpoint_halt;
  cmd.bIndex[0] = storage_device->endpoints[endpoint_type].number;

  uint8_t result = usb_control_transfer(storage_device, &cmd, (uint8_t *)0);

  storage_device->endpoints[endpoint_type].toggle = 0;

  CHECK(result);

  return result;
}
