#include "ch376.h"
#include "hw.h"
#include "usb.h"
#include "work-area.h"
#include <delay.h>
#include <stdbool.h>
#include <stdlib.h>

#include "debuggin.h"
#include "nextor.h"
#include "print.h"
#include <string.h>

usb_error usb_host_bus_reset() {
  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60 / 4);

  ch376_set_usb_mode(CH_MODE_HOST_RESET);
  delay(60 / 2);

  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60 / 4);

  ch_configure_nak_retry_indefinite();

  return USB_ERR_OK;
}

const endpoint_descriptor *parse_endpoint(_usb_state *const work_area, const endpoint_descriptor const *pEndpoint) {

  if (work_area->usb_device == USB_IS_FLOPPY) {
    if (pEndpoint->bmAttributes & 0x02) {
      if (pEndpoint->bmAttributes & 0x01) { // 3 -> Interrupt
        if (pEndpoint->bEndpointAddress & 0x80) {
          work_area->endpoints[ENDPOINT_INTERRUPT_IN].number          = pEndpoint->bEndpointAddress & 0x07;
          work_area->endpoints[ENDPOINT_INTERRUPT_IN].toggle          = 0;
          work_area->endpoints[ENDPOINT_INTERRUPT_IN].max_packet_size = pEndpoint->wMaxPacketSize;
        }
      } else {
        if (pEndpoint->bEndpointAddress & 0x80) {
          work_area->endpoints[ENDPOINT_BULK_IN].number          = pEndpoint->bEndpointAddress & 0x07;
          work_area->endpoints[ENDPOINT_BULK_IN].toggle          = 0;
          work_area->endpoints[ENDPOINT_BULK_IN].max_packet_size = pEndpoint->wMaxPacketSize;
        } else {
          work_area->endpoints[ENDPOINT_BULK_OUT].number          = pEndpoint->bEndpointAddress & 0x07;
          work_area->endpoints[ENDPOINT_BULK_OUT].toggle          = 0;
          work_area->endpoints[ENDPOINT_BULK_OUT].max_packet_size = pEndpoint->wMaxPacketSize;
        }
      }
    }
  }

  logEndPointDescription(pEndpoint);
  return pEndpoint + 1;
}

const interface_descriptor *parse_interface(_usb_state *const work_area, const interface_descriptor *p) {
  logInterface(p);

  work_area->interface_number = p->bInterfaceNumber;

  if (p->bInterfaceClass == 8 && p->bInterfaceSubClass == 6 && p->bInterfaceProtocol == 80 && work_area->usb_device == 0)
    work_area->usb_device = USB_IS_MASS_STORAGE;

  if (p->bInterfaceClass == 8 && p->bInterfaceSubClass == 4 && p->bInterfaceProtocol == 0 && work_area->usb_device == 0)
    work_area->usb_device = USB_IS_FLOPPY;

  const endpoint_descriptor *pEndpoint = (const endpoint_descriptor *)(p + 1);

  for (uint8_t endpoint_index = p->bNumEndpoints; endpoint_index != 0; endpoint_index--)
    pEndpoint = parse_endpoint(work_area, pEndpoint);

  return (interface_descriptor *)pEndpoint;
}

usb_error parse_config(_usb_state *const work_area, const device_descriptor *const desc, const uint8_t config_index) {
  uint16_t                 amount_transferred = 0;
  uint8_t                  result;
  uint8_t                  buffer[140];
  config_descriptor *const config_desc = (config_descriptor *)buffer;

  // printf("Config %d: ", config_index);

  result = hw_get_config_descriptor(
      config_desc, config_index, desc->bMaxPacketSize0, sizeof(config_descriptor), 0, &amount_transferred);
  if (result != USB_ERR_OK) {
    yprintf(15, "X1 (%d)", result);
    return result;
  }
  logConfig(config_desc);

  result = hw_get_config_descriptor(
      config_desc, config_index, desc->bMaxPacketSize0, config_desc->wTotalLength, 0, &amount_transferred);
  if (result != USB_ERR_OK) {
    yprintf(15, "X2 (%d)", result);
    return result;
  }

  work_area->bConfigurationvalue = config_desc->bConfigurationvalue;

  const interface_descriptor *p = (const interface_descriptor *)(buffer + sizeof(config_descriptor));
  for (uint8_t interface_index = 0; interface_index < config_desc->bNumInterfaces; interface_index++) {
    // printf("Interf %d: ", interface_index);
    p = parse_interface(work_area, p);

    if (work_area->usb_device)
      break;
  }

  return USB_ERR_OK;
}

usb_error read_all_configs(_usb_state *const work_area) {
  device_descriptor desc;
  uint8_t           result;

  result = hw_get_description(0, &desc);
  if (result != USB_ERR_OK)
    return result;

  printf("Desc: ");
  logDevice(&desc);

  work_area->usb_device      = 0;
  work_area->max_packet_size = desc.bMaxPacketSize0;

  for (uint8_t config_index = 0; config_index < desc.bNumConfigurations; config_index++) {
    if ((result = parse_config(work_area, &desc, config_index)) != USB_ERR_OK)
      return result;

    if (work_area->usb_device)
      break;
  }

  return USB_ERR_OK;
}

setup_packet cmd_port_power         = {0b00100011, 3, {8, 0}, {1, 0}, 0};
setup_packet cmd_port_reset         = {0b00100011, 3, {4, 0}, {1, 0}, 0};
setup_packet cmd_get_status_port    = {0b10100011, 0, {0, 0}, {1, 0}, 4};
setup_packet cmd_get_hub_descriptor = {0b10100000, 6, {0, 0x29}, {0, 0}, 8};

void hub_spike(_usb_state *const work_area) {
  usb_error result;
  uint16_t  amount = 0;
  uint8_t   buffer[10];
  uint8_t i;
  setup_packet cmd;
  memcpy(&cmd, &cmd_get_status_port, sizeof(setup_packet));

  if ((result = hw_set_address(2, work_area->max_packet_size)) != USB_ERR_OK) {
    printf("hub1 err: %d\r\n", result);
    return;
  }

  if ((result = usb_set_configuration(work_area->bConfigurationvalue, work_area->max_packet_size, 2)) != USB_ERR_OK) {
    printf("hub2 err: %d\r\n", result);
    return;
  }

  result = hw_control_transfer(&cmd_port_power, (uint8_t *)0, 2, 64, 0);

  if (result != USB_ERR_OK) {
    printf("hub3 err:%d\r\n", result);
    return;
  }

  result = hw_control_transfer(&cmd_get_hub_descriptor, buffer, 2, 64, &amount);
  printf("hub: %d, %d ", result, amount);
  for(i = 0; i < amount; i++)
    printf("%02X ", buffer[i]);

  printf("\r\n");



  // endpoint_param endpoint;
  // endpoint.number          = 0x81;
  // endpoint.toggle          = 0;
  // endpoint.max_packet_size = 2;

  // result = hw_data_in_transfer(buffer, 2, 2, &endpoint, &amount);
  // printf("INT IN %d, %d, %d %d\r\n", result, buffer[0], buffer[1], amount);

  result = hw_control_transfer(&cmd_port_reset, (uint8_t *)0, 2, 64, 0);

  if (result != USB_ERR_OK) {
    printf("hub4 err:%d\r\n", result);
    return;
  }

  for (i = 1; i <= 4; i++) {
    cmd.bIndex[0] = i;
    amount        = 0;
    result        = hw_control_transfer(&cmd, buffer, 2, 64, &amount);
    if (result != USB_ERR_OK) {
      printf("hub[%d] err: %d \r\n", i, result);
      return;
    }

    printf("stat: %d, %02x, %02x, %02x, %02x", amount, buffer[0], buffer[1], buffer[2], buffer[3]);
  }
}

uint8_t usb_host_init() {
  usb_error        result;
  work_area *const p = get_work_area();
  __asm__("EI");
  _usb_state *const work_area = &p->ch376;
  memset(work_area, 0, sizeof(_usb_state));

  ch376_reset();

  if (!ch376_probe()) {
    printf("CH376:           NOT PRESENT\r\n");
    return false;
  }

  const uint8_t ver = ch376_get_firmware_version();
  printf("CH376:           PRESENT (VER %d)\r\n", ver);

  usb_host_bus_reset();
  delay(10);

  read_all_configs(work_area);

  hub_spike(work_area);

  read_all_configs(work_area);

  switch (work_area->usb_device) {
  case USB_IS_FLOPPY:
    printf("USB:             FLOPPY\r\n");
    break;

  case USB_IS_MASS_STORAGE:
    printf("USB:             STORAGE\r\n");
    break;
  }

  if (p->ch376.usb_device) {
    if ((result = hw_set_address(DEVICE_ADDRESS, work_area->max_packet_size)) != USB_ERR_OK) {
      yprintf(15, "X2 (%d)", result);
      return result;
    }

    if ((result = usb_set_configuration(work_area->bConfigurationvalue, work_area->max_packet_size, DEVICE_ADDRESS)) !=
        USB_ERR_OK) {
      yprintf(15, "X3 (%d)", result);
      return result;
    }
  }

  return true;
}
