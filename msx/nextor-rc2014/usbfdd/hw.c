#include "hw.h"
#include "ch376.h"
#include "work-area.h"
#include <delay.h>
#include <stdlib.h>

#include "debuggin.h"
#include "print.h"

// ; The size and direction of the transfer are taken from the contents
// ; of the setup packet.
// ; -----------------------------------------------------------------------------
// ; Input:  cmd_packet => HL = Address of a 8 byte buffer with the setup packet
// ;         buffer => DE = Address of the input or output data buffer
// ;         device_address => A  = Device address
// ;         max_packet_size => B  = Maximum packet size for endpoint 0
// ; Output: A  = USB error code
// ;         BC = Amount of data actually transferred (if IN transfer and no error)

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
  CHECK(ch_short_wait_int_and_get_status());

  result = transferIn ? ch_data_in_transfer(buffer, cmd_packet->wLength, &endpoint)
                      : ch_data_out_transfer(buffer, cmd_packet->wLength, &endpoint);

  CHECK(result)

  if (transferIn) {
    ch_command(CH_CMD_WR_HOST_DATA);
    CH376_DATA_PORT = 0;
    ch_issue_token_out_ep0();
    result = ch_long_wait_int_and_get_status(); /* sometimes we get STALL here - seems to be ok to ignore */

    // printf(" E4(%d, %d) ", result, cmd_packet->wLength);
    if (result == USB_ERR_OK || result == USB_ERR_STALL)
      return USB_ERR_OK;

    CHECK(result);

    return result;
  }

  ch_issue_token_in_ep0();
  CHECK(ch_long_wait_int_and_get_status());

  return result;
}

const setup_packet cmd_get_device_descriptor = {0x80, 6, {0, 1}, {0, 0}, 8};

usb_error hw_get_description(device_descriptor *const buffer) {
  usb_error    result;
  setup_packet cmd;
  cmd         = cmd_get_device_descriptor;
  cmd.wLength = 8;

  CHECK(hw_control_transfer(&cmd, (uint8_t *)buffer, 0, 8));

  cmd         = cmd_get_device_descriptor;
  cmd.wLength = 18;
  CHECK(hw_control_transfer(&cmd, (uint8_t *)buffer, 0, buffer->bMaxPacketSize0));

  return result;
}

const setup_packet cmd_set_device_address = {0x00, 5, {0, 0}, {0, 0}, 0};

usb_error hw_set_address(const uint8_t device_address) __z88dk_fastcall {
  setup_packet cmd;
  cmd           = cmd_set_device_address;
  cmd.bValue[0] = device_address;

  return hw_control_transfer(&cmd, 0, 0, 0);
}

const setup_packet cmd_set_configuration = {0x00, 9, {0, 0}, {0, 0}, 0};

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

// ; -----------------------------------------------------------------------------
// ; HW_DATA_IN_TRANSFER: Perform a USB data IN transfer
// ; -----------------------------------------------------------------------------
// ; Input:  buffer => HL = Address of a buffer for the received data
// ;         buffer_size => BC = Data length
// ;         device_address => A  = Device address
// ;         max_packet_size => D  = Maximum packet size for the endpoint
// ;         endpoint => E  = Endpoint number
// ;         *toggle => Cy = Current state of the toggle bit
// ; Output: A  = USB error code
// ;         amount_received => BC = Amount of data actually received (only if no error)
// ;         *toggle Cy = New state of the toggle bit (even on error)

usb_error hw_data_in_transfer(uint8_t *             buffer,
                              const uint16_t        buffer_size,
                              const uint8_t         device_address,
                              endpoint_param *const endpoint) {

  ch_set_usb_address(device_address);

  return ch_data_in_transfer(buffer, buffer_size, endpoint);
}

// ; -----------------------------------------------------------------------------
// ; HW_DATA_OUT_TRANSFER: Perform a USB data OUT transfer
// ; -----------------------------------------------------------------------------
// ; Input:  HL = Address of a buffer for the data to be sent
// ;         BC = Data length
// ;         A  = Device address
// ;         D  = Maximum packet size for the endpoint
// ;         E  = Endpoint number
// ;         Cy = Current state of the toggle bit
// ; Output: A  = USB error code
// ;         Cy = New state of the toggle bit (even on error)
usb_error hw_data_out_transfer(const uint8_t *       buffer,
                               uint16_t              buffer_size,
                               const uint8_t         device_address,
                               endpoint_param *const endpoint) {

  ch_set_usb_address(device_address);

  return ch_data_out_transfer(buffer, buffer_size, endpoint);
}
