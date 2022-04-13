#include "usb-enumerate-hub.h"
#include "hw.h"
#include "usb-enumerate.h"
#include <delay.h>
#include <string.h>

#include "print.h"

void parse_endpoint_hub(const endpoint_descriptor const *pEndpoint) __z88dk_fastcall {
  _usb_state *const work_area = get_usb_work_area();

  work_area->hub_endpoint.number          = pEndpoint->bEndpointAddress;
  work_area->hub_endpoint.toggle          = 0;
  work_area->hub_endpoint.max_packet_size = pEndpoint->wMaxPacketSize;
}

const setup_packet cmd_set_feature = {RT_HOST_TO_DEVICE | RT_CLASS | RT_OTHER, SET_FEATURE, {FEAT_PORT_POWER, 0}, {1, 0}, 0};
const setup_packet cmd_clear_feature = {
    RT_HOST_TO_DEVICE | RT_CLASS | RT_OTHER, CLEAR_FEATURE, {FEAT_PORT_POWER, 0}, {1, 0}, 0};
const setup_packet cmd_get_status_port = {
    RT_DEVICE_TO_HOST | RT_CLASS | RT_OTHER, GET_STATUS, {0, 0}, {1, 0}, sizeof(hub_port_status)};
const setup_packet cmd_get_hub_descriptor = {RT_DEVICE_TO_HOST | RT_CLASS | RT_DEVICE, 6, {0, 0x29}, {0, 0}, 8};

usb_error hub_set_feature(const uint8_t feature, const uint8_t index) {
  setup_packet set_feature;
  set_feature = cmd_set_feature;

  set_feature.bValue[0] = feature;
  set_feature.bIndex[0] = index;
  return hw_control_transfer(&set_feature, 0, DEVICE_ADDRESS_HUB, get_usb_work_area()->hub_config.max_packet_size);
}

usb_error hub_clear_feature(const uint8_t feature, const uint8_t index) {
  setup_packet clear_feature;
  clear_feature = cmd_clear_feature;

  clear_feature.bValue[0] = feature;
  clear_feature.bIndex[0] = index;
  return hw_control_transfer(&clear_feature, 0, DEVICE_ADDRESS_HUB, get_usb_work_area()->hub_config.max_packet_size);
}

usb_error hub_get_status_port(const uint8_t index, hub_port_status *const port_status) {
  setup_packet get_status_port;
  get_status_port = cmd_get_status_port;

  get_status_port.bIndex[0] = index;
  return hw_control_transfer(
      &get_status_port, port_status, DEVICE_ADDRESS_HUB, get_usb_work_area()->hub_config.max_packet_size);
}

usb_error hub_get_descriptor(hub_descriptor *const hub_description) __z88dk_fastcall {
  return hw_control_transfer(
      &cmd_get_hub_descriptor, hub_description, DEVICE_ADDRESS_HUB, get_usb_work_area()->hub_config.max_packet_size);
}

usb_error configure_usb_hub(_working *const working) __z88dk_fastcall {
  _usb_state *const work_area = get_usb_work_area();

  usb_error       result;
  uint8_t         i;
  hub_descriptor  hub_description;
  hub_port_status port_status;

  work_area->hub_config.address = DEVICE_ADDRESS_HUB;

  CHECK(hw_set_address_and_configuration(&work_area->hub_config));

  CHECK(hub_get_descriptor(&hub_description));

  for (i = 1; i <= hub_description.bNbrPorts; i++) {
    printf("Scanning %d ", i);
    CHECK(hub_set_feature(FEAT_PORT_POWER, i));

    CHECK(hub_set_feature(FEAT_PORT_RESET, i), x_printf("hub4 err:%d\r\n", result));

    CHECK(hub_get_status_port(i, &port_status), x_printf("hub[%d] err: %d \r\n", 0, result));

    if (port_status.wPortStatus.port_connection) {
      delay(5);
      printf("Connected");
      CHECK(read_all_configs(&working->next_storage_device_index), x_printf("err5: %d\r\n", result));
    } else {
      printf("Not Connected");
    }

    if (work_area->xusb_device != USB_IS_HUB)
      break;

    printf("clearing");
    CHECK(hub_clear_feature(FEAT_PORT_POWER, i), x_printf("hub5 err:%d\r\n", result));

    printf("\r\n");
  }

  return USB_ERR_OK;
}
