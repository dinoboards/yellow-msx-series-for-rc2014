#include "class_printer.h"
#include "protocol.h"
#include <string.h>

const setup_packet cmd_get_port_status = {0xA1, 1, {0, 0}, {0, 0}, 1};

usb_error prt_get_port_status(const device_config_printer *const dev, uint8_t *buffer) {
  usb_error    result;
  setup_packet cmd;
  cmd = cmd_get_port_status;

  cmd.bIndex[1] = dev->interface_number;

  result = usb_control_transfer(&cmd, (uint8_t *)buffer, dev->address, dev->max_packet_size);

  CHECK(result);

  return result;
}

const setup_packet cmd_get_device_id = {0xA1, 0, {0, 0}, {0, 0}, 4};

usb_error prt_get_device_id(const device_config_printer *const dev, uint8_t *buffer) {
  usb_error    result;
  setup_packet cmd;
  cmd = cmd_get_device_id;

  result = usb_control_transfer(&cmd, (uint8_t *)buffer, dev->address, dev->max_packet_size);

  CHECK(result);

  return result;
}

const setup_packet cmd_soft_reset = {0x21, 2, {0, 0}, {0, 0}, 0};

usb_error prt_soft_reset(const device_config_printer *const dev) {
  usb_error    result;
  setup_packet cmd;
  cmd = cmd_soft_reset;

  result = usb_control_transfer(&cmd, 0, dev->address, dev->max_packet_size);

  CHECK(result);

  return result;
}

usb_error prt_send_text(device_config_printer *dev, const uint8_t *text, const uint8_t length) {
  usb_error result;

  CHECK(usb_data_out_transfer(text, length, dev->address, &dev->endpoints[0]));

  return result;
}
