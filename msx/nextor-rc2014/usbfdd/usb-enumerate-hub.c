#include "usb-enumerate-hub.h"
#include "hw.h"
#include "usb-enumerate.h"
#include <string.h>

void parse_endpoint_hub(_usb_state *const work_area, const endpoint_descriptor const *pEndpoint) {
  work_area->hub_endpoint.number          = pEndpoint->bEndpointAddress;
  work_area->hub_endpoint.toggle          = 0;
  work_area->hub_endpoint.max_packet_size = pEndpoint->wMaxPacketSize;
}

setup_packet cmd_port_power         = {0b00100011, 3, {8, 0}, {1, 0}, 0};
setup_packet cmd_port_reset         = {0b00100011, 3, {4, 0}, {1, 0}, 0};
setup_packet cmd_get_status_port    = {0b10100011, 0, {0, 0}, {1, 0}, 4};
setup_packet cmd_get_hub_descriptor = {0b10100000, 6, {0, 0x29}, {0, 0}, 8};

void configure_usb_hub(_usb_state *const work_area) {
  usb_error    result;
  uint8_t      buffer[10];
  uint8_t      i;
  setup_packet cmd;
  memcpy(&cmd, &cmd_get_status_port, sizeof(setup_packet));

  if ((result = hw_set_address(DEVICE_ADDRESS_HUB, work_area->max_packet_size)) != USB_ERR_OK) {
    // printf("hub1 err: %d\r\n", result);
    return;
  }

  if ((result = usb_set_configuration(work_area->bConfigurationvalue, work_area->max_packet_size, DEVICE_ADDRESS_HUB)) != USB_ERR_OK) {
    // printf("hub2 err: %d\r\n", result);
    return;
  }

  result = hw_control_transfer(&cmd_port_power, (uint8_t *)0, DEVICE_ADDRESS_HUB, work_area->max_packet_size);

  if (result != USB_ERR_OK) {
    // printf("hub3 err:%d\r\n", result);
    return;
  }

TODO:
 1 - extract port count from response structure
 2 - loop for number of product_revision
 3 - update procedure to activate each port one by one - device should be able to be plug into any port
 
  result = hw_control_transfer(&cmd_get_hub_descriptor, buffer, DEVICE_ADDRESS_HUB, work_area->max_packet_size);
  // printf("hub: %d ", result);
  // for (i = 0; i < 8; i++)
  //   printf("%02X ", buffer[i]);

  // printf("\r\n");

  // endpoint_param endpoint;
  // endpoint.number          = 0x81;
  // endpoint.toggle          = 0;
  // endpoint.max_packet_size = 2;

  // result = hw_data_in_transfer(buffer, 2, 2, &endpoint, &amount);
  // printf("INT IN %d, %d, %d %d\r\n", result, buffer[0], buffer[1], amount);

  result = hw_control_transfer(&cmd_port_reset, (uint8_t *)0, DEVICE_ADDRESS_HUB, work_area->max_packet_size);

  if (result != USB_ERR_OK) {
    // printf("hub4 err:%d\r\n", result);
    return;
  }

  for (i = 1; i <= 4; i++) {
    cmd.bIndex[0] = i;
    result        = hw_control_transfer(&cmd, buffer, DEVICE_ADDRESS_HUB, work_area->max_packet_size);
    if (result != USB_ERR_OK) {
      // printf("hub[%d] err: %d \r\n", i, result);
      return;
    }

    // printf("stat: %d, %02x, %02x, %02x", buffer[0], buffer[1], buffer[2], buffer[3]);
  }

  read_all_configs(work_area);
}
