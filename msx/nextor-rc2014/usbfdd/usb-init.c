#include "ch376.h"
#include "hw.h"
#include "usb.h"
#include "work-area.h"
#include <delay.h>
#include <stdbool.h>
#include <stdlib.h>

#include "debuggin.h"
#include "print.h"

inline void hw_configure_nak_retry() {
  setCommand(CH_CMD_SET_RETRY);
  CH376_DATA_PORT = 0x25;
  CH376_DATA_PORT = 0x8F; // Retry NAKs indefinitely (default)
}

uint8_t usb_host_bus_reset() {
  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60 / 4);

  ch376_set_usb_mode(CH_MODE_HOST_RESET);
  delay(60 / 2);

  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60 / 4);

  hw_configure_nak_retry();

  return true;
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

  for (uint8_t endpoint_index = 0; endpoint_index < p->bNumEndpoints; endpoint_index++) {
    printf("end %p: ", endpoint_index);

    pEndpoint = parse_endpoint(work_area, pEndpoint);
  }

  return (interface_descriptor *)pEndpoint;
}

uint8_t parse_config(_usb_state *const work_area, const device_descriptor *const desc, const uint8_t config_index) {
  uint16_t                 amount_transferred = 0;
  uint8_t                  result;
  uint8_t                  buffer[140];
  config_descriptor *const config_desc = (config_descriptor *)buffer;

  printf("Config %d: ", config_index);

  result = hw_get_config_descriptor(
      config_desc, config_index, desc->bMaxPacketSize0, sizeof(config_descriptor), 0, &amount_transferred);
  if (result != CH_USB_INT_SUCCESS) {
    yprintf(15, "X1 (%d)", result);
    return result;
  }
  logConfig(config_desc);

  result = hw_get_config_descriptor(
      config_desc, config_index, desc->bMaxPacketSize0, config_desc->wTotalLength, 0, &amount_transferred);
  if (result != CH_USB_INT_SUCCESS) {
    yprintf(15, "X2 (%d)", result);
    return result;
  }

  work_area->bConfigurationvalue = config_desc->bConfigurationvalue;

  const interface_descriptor *p = (const interface_descriptor *)(buffer + sizeof(config_descriptor));
  for (uint8_t interface_index = 0; interface_index < config_desc->bNumInterfaces; interface_index++) {
    printf("Interf %d: ", interface_index);
    p = parse_interface(work_area, p);

    if (work_area->usb_device)
      break;
  }

  return CH_USB_INT_SUCCESS;
}

uint8_t read_all_configs(_usb_state *const work_area) {
  device_descriptor desc;
  uint8_t           result;

  result = hw_get_description(0, &desc);
  printf("Desc %02x\r\n", result);
  logDevice(&desc);

  work_area->usb_device      = 0;
  work_area->max_packet_size = desc.bMaxPacketSize0;

  for (uint8_t config_index = 0; config_index < desc.bNumConfigurations; config_index++) {
    if ((result = parse_config(work_area, &desc, config_index)) != CH_USB_INT_SUCCESS)
      return result;

    if (work_area->usb_device)
      break;
  }

  return CH_USB_INT_SUCCESS;
}

uint8_t usb_host_init() {
  __asm__("EI");
  uint8_t           result;
  work_area *const  p         = get_work_area();
  _usb_state *const work_area = &p->ch376;

  printf("usb_host_init %p\r\n", p);

  ch376_reset();

  if (!ch376_probe()) {
    printf("CH376:           NOT PRESENT\r\n");
    return false;
  }

  // p->ch376.present  = true;
  const uint8_t ver = ch376_get_firmware_version();
  printf("CH376:           PRESENT (VER %d)\r\n", ver);

  usb_host_bus_reset();
  delay(10);

  read_all_configs(&p->ch376);

  if (p->ch376.usb_device) {
    if ((result = hw_set_address(DEVICE_ADDRESS, work_area->max_packet_size)) != CH_USB_INT_SUCCESS) {
      yprintf(15, "X2 (%d)", result);
      return result;
    }

    if ((result = usb_set_configuration(work_area->bConfigurationvalue, work_area->max_packet_size, DEVICE_ADDRESS)) !=
        CH_USB_INT_SUCCESS) {
      yprintf(15, "X3 (%d)", result);
      return result;
    }
  }

  logWorkArea(&p->ch376);

  return true;
}
