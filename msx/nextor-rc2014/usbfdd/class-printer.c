#include "class-printer.h"
#include "hw.h"
#include <string.h>
#include "print.h"

const setup_packet cmd_get_port_status = {0xA1, 1, {0, 0}, {0, 0}, 1};

usb_error prt_get_port_status(const device_config *const dev, uint8_t *buffer) {
  usb_error    result;
  setup_packet cmd;
  cmd = cmd_get_port_status;

  result = hw_control_transfer(&cmd, (uint8_t *)buffer, dev->address, dev->max_packet_size);

  CHECK(result);

  return result;
}

const setup_packet cmd_get_device_id = {0xA1, 0, {0, 0}, {0, 0}, 4};

usb_error prt_get_device_id(const device_config *const dev, uint8_t *buffer) {
  usb_error    result;
  setup_packet cmd;
  cmd = cmd_get_device_id;

  result = hw_control_transfer(&cmd, (uint8_t *)buffer, dev->address, dev->max_packet_size);

  CHECK(result);

  return result;
}

const setup_packet cmd_soft_reset = {0x21, 2, {0, 0}, {0, 0}, 0};

usb_error prt_soft_reset(const device_config *const dev) {
  usb_error    result;
  setup_packet cmd;
  cmd = cmd_soft_reset;

  result = hw_control_transfer(&cmd, 0, dev->address, dev->max_packet_size);

  CHECK(result);

  return result;
}

usb_error prt_send_text(device_config *const dev, const char *text) {
  usb_error result;

  dev->endpoints[0].toggle = 0;

  CHECK(hw_data_out_transfer((const uint8_t *)text, strlen(text), dev->address, &dev->endpoints[0]));

  return result;
}
