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
    ch_write_datax();
    ch_issue_token_out_ep0();
    CHECK(ch_long_wait_int_and_get_status(), x_printf(" E4(%d) ", result));
    return result;
  }

  ch_issue_token_in_ep0();
  result = ch_long_wait_int_and_get_status();

  if (result == USB_ERR_NAK) {
    ch_issue_token_in_ep0();
    CHECK(ch_long_wait_int_and_get_status(), x_printf(" E5 %d ", result));
  }

  ch_read_datax();

  return result;
}

usb_error hw_get_description(device_descriptor *const buffer) {
  ch_set_usb_address(0);

  usb_error result;

  CHECK(ch_control_transfer_request_descriptor(1));

  uint8_t amount_received;
  ch_read_data((uint8_t *)buffer, 18, &amount_received);

  if (amount_received != 18)
    return USB_ERR_DATA_ERROR;

  return USB_ERR_OK;
}

usb_error hw_get_config_descriptor(config_descriptor *const buffer,
                                   const uint8_t            config_index,
                                   const uint8_t            buffer_size,
                                   const uint8_t            device_address) {

  usb_error result;

  if (config_index != 0)
    return USB_ERR_FAIL;

  ch_set_usb_address(device_address);
  CHECK(ch_control_transfer_request_descriptor(2));

  uint8_t amount_received;
  ch_read_data((uint8_t *)buffer, buffer_size, &amount_received);

  return USB_ERR_OK;
}

#define PLACEHOLDER_TARGET_DEVICE_ADDRESS 0
setup_packet cmd_set_address = {0x00, 0x05, {PLACEHOLDER_TARGET_DEVICE_ADDRESS, 0}, {0, 0}, 0};

usb_error hw_set_configuration(const device_config *const config) {
  setup_packet cmd;

  cmd           = cmd_set_address;
  cmd.bRequest  = 0x09;
  cmd.bValue[0] = config->value;
  return hw_control_transfer(&cmd, (uint8_t *)0, config->address, config->max_packet_size);
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
