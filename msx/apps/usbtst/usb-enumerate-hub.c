#include "usb-enumerate-hub.h"
#include "hw.h"
#include "usb-enumerate.h"
#include <delay.h>
#include <string.h>

#include "debuggin.h"
#include "print.h"

void parse_endpoint_hub(const endpoint_descriptor const *pEndpoint) __z88dk_fastcall {
  _usb_state *const work_area = get_usb_work_area();

  work_area->hub_endpoint.number = pEndpoint->bEndpointAddress;
  work_area->hub_endpoint.toggle = 0;
  work_area->hub_endpoint.max_packet_size = pEndpoint->wMaxPacketSize;
}

const setup_packet cmd_set_feature = {RT_HOST_TO_DEVICE | RT_CLASS | RT_OTHER, SET_FEATURE, {FEAT_PORT_POWER, 0}, {1, 0}, 0};
const setup_packet cmd_clear_feature = {RT_HOST_TO_DEVICE | RT_CLASS | RT_OTHER, CLEAR_FEATURE, {FEAT_PORT_POWER, 0}, {1, 0}, 0};
const setup_packet cmd_get_status_port = {RT_DEVICE_TO_HOST | RT_CLASS | RT_OTHER, GET_STATUS, {0, 0}, {1, 0}, sizeof(hub_port_status)};
const setup_packet cmd_get_hub_descriptor = {RT_DEVICE_TO_HOST | RT_CLASS | RT_DEVICE, 6, {0, 0x29}, {0, 0}, 8};

usb_error hub_set_feature(const uint8_t feature, const uint8_t index) {
  _usb_state *const work_area = get_usb_work_area();

  setup_packet set_feature;
  set_feature = cmd_set_feature;

  set_feature.bValue[0] = feature;
  set_feature.bIndex[0] = index;
  return hw_control_transfer(&set_feature, 0, work_area->hub_config.address, work_area->hub_config.max_packet_size);
}

usb_error hub_clear_feature(const uint8_t feature, const uint8_t index) {
  _usb_state *const work_area = get_usb_work_area();

  setup_packet clear_feature;
  clear_feature = cmd_clear_feature;

  clear_feature.bValue[0] = feature;
  clear_feature.bIndex[0] = index;
  return hw_control_transfer(&clear_feature, 0, work_area->hub_config.address, work_area->hub_config.max_packet_size);
}

usb_error hub_get_status_port(const uint8_t index, hub_port_status *const port_status) {
  _usb_state *const work_area = get_usb_work_area();

  setup_packet get_status_port;
  get_status_port = cmd_get_status_port;

  get_status_port.bIndex[0] = index;
  return hw_control_transfer(&get_status_port, port_status, work_area->hub_config.address, work_area->hub_config.max_packet_size);
}

usb_error hub_get_descriptor(hub_descriptor *const hub_description) __z88dk_fastcall {
  _usb_state *const work_area = get_usb_work_area();

  return hw_control_transfer(&cmd_get_hub_descriptor, hub_description, work_area->hub_config.address, work_area->hub_config.max_packet_size);
}

usb_error configure_usb_hub(_working *const working) __z88dk_fastcall {
  _usb_state *const work_area = get_usb_work_area();

  usb_error       result;
  hub_descriptor  hub_description;
  hub_port_status port_status;

  CHECK(hw_set_configuration(&work_area->hub_config));

  CHECK(hub_get_descriptor(&hub_description));

  logHubDescription(&hub_description);

  uint8_t i = hub_description.bNbrPorts;
  do {
    CHECK(hub_clear_feature(FEAT_PORT_POWER, i));
    delay_short();

    CHECK(hub_set_feature(FEAT_PORT_POWER, i));
    delay_short();

    hub_clear_feature(FEAT_PORT_RESET, i);
    delay_short();

    CHECK(hub_set_feature(FEAT_PORT_RESET, i));
    delay_short();

    CHECK(hub_get_status_port(i, &port_status));

    if (port_status.wPortStatus.port_connection) {
      CHECK(hub_clear_feature(HUB_FEATURE_PORT_CONNECTION_CHANGE, i));
      delay_short();

      CHECK(hub_clear_feature(FEAT_PORT_ENABLE_CHANGE, i));
      delay_short();

      CHECK(hub_clear_feature(FEAT_PORT_RESET_CHANGE, i));
      delay_short();

      CHECK(hub_get_status_port(i, &port_status));

      CHECK(read_all_configs(working->state));

    } else {
      CHECK(hub_clear_feature(FEAT_PORT_POWER, i));
    }
  } while (--i != 0);

  return USB_ERR_OK;
}
