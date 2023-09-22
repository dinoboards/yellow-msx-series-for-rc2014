/**
 * @file hw.c
 * @author Dean Netherton
 * @brief A simplest implementation of common usb transfer functions, based on the CH376S chip
 * @details For a basic walkthrough of the usb protocol see https://www.beyondlogic.org/usbnutshell/usb1.shtml
 * @version 1.0
 * @date 2023-09-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "ch376.h"
#include "hw.h"
#include <delay.h>
#include <stdlib.h>

/**
 * @brief Perform a USB control transfer (in or out)
 * See https://www.beyondlogic.org/usbnutshell/usb4.shtml for a description of the USB control transfer
 *
 * @param cmd_packet Pointer to the setup packet - top bit of bmRequestType indicate data direction
 * @param buffer Pointer of data to send or receive into
 * @param device_address usb device address
 * @param max_packet_size Maximum packet size for endpoint
 * @return usb_error USB_ERR_OK if all good, otherwise specific error code
 */
usb_error hw_control_transfer(const setup_packet *const cmd_packet,
                              void *const               buffer,
                              const uint8_t             device_address,
                              const uint8_t             max_packet_size) {
  usb_error      result;
  endpoint_param endpoint = {1, 0, max_packet_size};

  const uint8_t transferIn = (cmd_packet->bmRequestType & 0x80);

  if (transferIn && buffer == 0)
    return USB_ERR_OTHER;

  ch_set_usb_address(device_address);

  ch_write_data((const uint8_t *)cmd_packet, sizeof(setup_packet));
  ch_issue_token_setup();
  CHECK(ch_short_wait_int_and_get_status())

  const uint16_t length = cmd_packet->wLength;

  result = length != 0
               ? (transferIn ? ch_data_in_transfer(buffer, length, &endpoint) : ch_data_out_transfer(buffer, length, &endpoint))
               : USB_ERR_OK;

  CHECK(result)

  if (transferIn) {
    ch_command(CH_CMD_WR_HOST_DATA);
    CH376_DATA_PORT = 0;
    ch_issue_token_out_ep0();
    result = ch_long_wait_int_and_get_status(); /* sometimes we get STALL here - seems to be ok to ignore */

    if (result == USB_ERR_OK || result == USB_ERR_STALL)
      return USB_ERR_OK;

    CHECK(result);

    return result;
  }

  ch_issue_token_in_ep0();
  result = ch_long_wait_int_and_get_status();

  if (result == USB_ERR_STALL)
    return USB_ERR_STALL;

  CHECK(result);

  return result;
}

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

//  -----------------------------------------------------------------------------
//  Perform a USB data IN transfer
//  -----------------------------------------------------------------------------
//  Input:  buffer => Address of a buffer for the received data
//          buffer_size => Data length
//          device_address =>  Device address
//          endpoint => Endpoint number description (including toggle)
//  Output: result USB error code
//           endpoint's toggle

usb_error
hw_data_in_transfer(uint8_t *buffer, const uint16_t buffer_size, const uint8_t device_address, endpoint_param *const endpoint) {
  usb_error result;
  ch_set_usb_address(device_address);

  CHECK(ch_data_in_transfer(buffer, buffer_size, endpoint));

  return result;
}

// -----------------------------------------------------------------------------
// Perform a USB data OUT transfer
// -----------------------------------------------------------------------------
// Input:   buffer Address of a buffer for the data to be sent
//          buffer_size = Data length
//          device_address  = Device address
//          endpoint => Endpoint number description (including toggle)
//  Output: result USB error code
//           endpoint's toggle
usb_error
hw_data_out_transfer(const uint8_t *buffer, uint16_t buffer_size, const uint8_t device_address, endpoint_param *const endpoint) {

  ch_set_usb_address(device_address);

  return ch_data_out_transfer(buffer, buffer_size, endpoint);
}
