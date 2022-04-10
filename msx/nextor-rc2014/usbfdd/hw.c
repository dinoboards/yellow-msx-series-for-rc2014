#include "hw.h"
#include "ch376.h"
#include "work-area.h"
#include <delay.h>
#include <stdlib.h>

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

  if (transferIn && buffer == 0) {
    x_printf(" !!Err1 ");
    return USB_ERR_OTHER;
  }

  ch_command(CH_CMD_SET_USB_ADDR);
  CH376_DATA_PORT = device_address;

  ch_write_data((const uint8_t *)cmd_packet, sizeof(setup_packet));

  ch_issue_token_setup();

  CHECK(ch_short_wait_int_and_get_status(), x_printf("Tok1 %02x\r\n", result));

  result = transferIn ? ch_data_in_transfer(buffer, cmd_packet->wLength, &endpoint)
                      : ch_data_out_transfer(buffer, cmd_packet->wLength, &endpoint);

  if (result != USB_ERR_OK) {
    x_printf(" Err2 (%d, %d, %d, %d) ", transferIn, cmd_packet->wLength, result, max_packet_size);
    return result;
  }

  delay(6);

  if (transferIn) {
    ch_write_data((const uint8_t *)0, 0);
    ch_issue_token_out_ep0();
    CHECK(ch_long_wait_int_and_get_status(), x_printf(" E4(%d) ", result));
    return result;
  }

  ch_issue_token_in_ep0();
  CHECK(ch_long_wait_int_and_get_status(), x_printf(" E5 %d ", result));

  return result;
}

setup_packet cmd_get_device_descriptor = {0x80, 6, {0, 1}, {0, 0}, 18};

usb_error hw_get_description(const uint8_t device_address, const uint8_t max_packet_size, device_descriptor *const buffer) {
  usb_error result;

  result = hw_control_transfer(&cmd_get_device_descriptor, (uint8_t *)buffer, device_address, max_packet_size);

  if (result != USB_ERR_OK)
    return result;

  return USB_ERR_OK;
}

#define PLACEHOLDER_CONFIGURATION_ID       0
#define PLACEHOLDER_CONFIG_DESCRIPTOR_SIZE (sizeof(config_descriptor))

setup_packet cmd_get_config_descriptor = {
    0x80, 6, {PLACEHOLDER_CONFIGURATION_ID, 2}, {0, 0}, PLACEHOLDER_CONFIG_DESCRIPTOR_SIZE};

usb_error hw_get_config_descriptor(config_descriptor *const buffer,
                                   const uint8_t            config_index,
                                   const uint8_t            max_packet_size,
                                   const uint8_t            buffer_size,
                                   const uint8_t            device_address) {

  setup_packet cmd;
  cmd = cmd_get_config_descriptor;

  cmd.bValue[0] = config_index;
  cmd.wLength   = (uint16_t)buffer_size;

  return hw_control_transfer(&cmd, (uint8_t *)buffer, device_address, max_packet_size);
}

#define PLACEHOLDER_TARGET_DEVICE_ADDRESS 0
setup_packet cmd_set_address = {0x00, 0x05, {PLACEHOLDER_TARGET_DEVICE_ADDRESS, 0}, {0, 0}, 0};

// #define PLACEHOLDER_CONFIGURATION_VALUE 0
// setup_packet usb_cmd_set_configuration = {0x00, 0x09, {PLACEHOLDER_CONFIGURATION_VALUE, 0}, {0, 0}, 0};

usb_error hw_set_address_and_configuration(const uint8_t usb_address, const device_config *const config) {
  setup_packet cmd;
  usb_error    result;

  cmd           = cmd_set_address;
  cmd.bValue[0] = usb_address;
  CHECK(hw_control_transfer(&cmd, (uint8_t *)0, 0, config->max_packet_size));

  cmd.bRequest  = 0x09;
  cmd.bValue[0] = config->value;
  return hw_control_transfer(&cmd, (uint8_t *)0, usb_address, config->max_packet_size);
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

  ch_command(CH_CMD_SET_USB_ADDR);
  CH376_DATA_PORT = device_address;

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

  ch_command(CH_CMD_SET_USB_ADDR);
  CH376_DATA_PORT = device_address;

  return ch_data_out_transfer(buffer, buffer_size, endpoint);
}
