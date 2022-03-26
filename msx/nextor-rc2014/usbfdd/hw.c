#include "hw.h"
#include "ch376.h"
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
                              uint8_t *const            buffer,
                              const uint8_t             device_address,
                              const uint8_t             max_packet_size,
                              uint16_t *const           amount_transferred) {
  usb_error result;
  uint8_t   toggle;

retry:
  toggle = 1;
  setCommand(CH_CMD_SET_USB_ADDR);
  CH376_DATA_PORT = device_address;

  ch_write_data((const uint8_t *)cmd_packet, sizeof(setup_packet));

  ch_issue_token(0, CH_PID_SETUP, 0);

  if ((result = ch_wait_int_and_get_result()) != USB_ERR_OK) {
    yprintf(10, "Err1 (%d) ", result);
    return result;
  }

  const uint8_t transferIn = (cmd_packet->bmRequestType & 0x80);

  if (transferIn && buffer == 0) {
    yprintf(10, "Err2 ");
    return 98;
  }

  result = transferIn ? ch_data_in_transfer(buffer, cmd_packet->wLength, max_packet_size, 0, amount_transferred, &toggle)
                      : ch_data_out_transfer(buffer, cmd_packet->wLength, max_packet_size, 0, &toggle);

  if (result != USB_ERR_OK) {
    yprintf(10, "Err3 (%d) ", result);
    return result;
  }

  if (transferIn) {
    ch_write_data((const uint8_t *)0, 0);
    ch_issue_token(0, CH_PID_OUT, 0x40);
  } else
    ch_issue_token(0, CH_PID_IN, 0x80);

  result = ch_wait_int_and_get_result();

  if (transferIn)
    return result;

  ch_read_data((uint8_t *)0, 0, 0);
  return ch_wait_int_and_get_result();
}

setup_packet cmd_get_device_descriptor = {0x80, 6, {0, 1}, {0, 0}, 18};

usb_error hw_get_description(const uint8_t device_address, device_descriptor *const buffer) {
  usb_error result;
  uint16_t  amount_received = 0;

  result = hw_control_transfer(
      &cmd_get_device_descriptor, (uint8_t *)buffer, device_address, sizeof(device_descriptor), &amount_received);

  if (result != USB_ERR_OK)
    return result;

  printf("received = %d\r\n", amount_received);

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
                                   const uint8_t            device_address,
                                   uint16_t *const          amount_transferred) {

  setup_packet cmd;
  cmd                 = cmd_get_config_descriptor;
  *amount_transferred = 0;

  cmd.bValue[0] = config_index;
  cmd.wLength   = (uint16_t)buffer_size;

  return hw_control_transfer(&cmd, (uint8_t *)buffer, device_address, max_packet_size, amount_transferred);
}

#define PLACEHOLDER_TARGET_DEVICE_ADDRESS 0
setup_packet cmd_set_address = {0x00, 0x05, {PLACEHOLDER_TARGET_DEVICE_ADDRESS, 0}, {0, 0}, 0};

usb_error hw_set_address(const uint8_t usb_address, const uint8_t packet_size) {
  uint16_t amount_transferred = 0;

  setup_packet cmd;
  cmd           = cmd_set_address;
  cmd.bValue[0] = usb_address;

  return hw_control_transfer(&cmd, (uint8_t *)0, 0, packet_size, &amount_transferred);
}

#define PLACEHOLDER_CONFIGURATION_VALUE 0
setup_packet usb_cmd_set_configuration = {0x00, 0x09, {PLACEHOLDER_CONFIGURATION_VALUE, 0}, {0, 0}, 0};

usb_error usb_set_configuration(const uint8_t configuration_value, const uint8_t packet_size, const uint8_t device_address) {
  uint16_t amount_transferred = 0;

  setup_packet cmd;
  cmd           = usb_cmd_set_configuration;
  cmd.bValue[0] = configuration_value;

  return hw_control_transfer(&cmd, (uint8_t *)0, device_address, packet_size, &amount_transferred);
}
