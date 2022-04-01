#include "ch376.h"
#include "hw.h"
#include "usb.h"
#include "work-area.h"
#include <delay.h>
#include <stdbool.h>
#include <stdlib.h>

#include "debuggin.h"
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

  // logEndPointDescription(pEndpoint);
  return pEndpoint + 1;
}

const interface_descriptor *parse_interface(_usb_state *const work_area, const interface_descriptor *p) {
  // logInterface(p);

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
  // logConfig(config_desc);

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
  // printf("Desc %02x\r\n", result);
  // logDevice(&desc);

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

uint8_t usb_host_init() {
  usb_error        result;
  work_area *const p = get_work_area();
  __asm__("EI");
  _usb_state *const work_area = &p->ch376;
  memset(work_area, 0, sizeof(_usb_state));

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

  read_all_configs(work_area);

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

  // logWorkArea(work_area);
  ufi_inquiry_response inq_response;
  memset(&inq_response, 0, sizeof(ufi_inquiry_response));

  result = ufi_inquiry(work_area, &inq_response);
  // printf("inq: %02x\r\n", result);
  // logInquiryResponse(&inq_response);

  printf(">");
  result = test_disk(work_area);
  printf("test=%d\r\n", result);
  printf("<");

  ufi_format_capacities_response cap_response;
  result = ufi_capacity(work_area, &cap_response);
  printf("%d,%d\r\n", result, cap_response.descriptor_code);
  // ;Useful information returned by the Read Format Capacities command:
  // ;+6: High byte of disk capacity in sectors:
  // ;    5h: 720K
  // ;    4h: 1.25M
  // ;    Bh: 1.44M
  // ;+8: Disk format status:
  // ;    01b: unformatted
  // ;    10b: formatted
  // ;    11b: no disk in drive

  // printf("CAP: ");
  // switch (cap_response.descriptor_code) {
  // case 1:
  //   printf("unformatted, ");
  //   break;

  // case 2:
  //   printf("formatted, ");
  //   break;

  // case 3:
  //   printf("no disk, ");
  // }

  // switch (cap_response.number_of_blocks[2]) {
  // case 5:
  //   printf("720K");
  //   break;

  // case 4:
  //   printf("1.25M");
  //   break;

  // case 0x0B:
  //   printf("1.44MB");
  // }
  // printf("\r\n");

  printf(">");
  result = test_disk(work_area);
  printf("test=%d\r\n", result);
  printf("<");
  ufi_read_sector(work_area, 0);

  printf(">");
  result = test_disk(work_area);
  printf("test=%d\r\n", result);
  printf("<");
  ufi_write_sector(work_area, 2);
  long_delay(8);

  // full_reset(work_area);

  // printf("> ");
  // test_disk(work_area);
  // printf("<");

  printf(">>");
  result = test_disk(work_area);
  printf("test=%d\r\n", result);
  printf("<<");
  ufi_read_sector(work_area, 2);

  printf(">>>");
  test_disk(work_area);
  printf("<<<");
  ufi_write_sector(work_area, 1);

  printf(">");
  result = test_disk(work_area);
  printf("test=%d\r\n", result);
  printf("<");
  ufi_read_sector(work_area, 1);

  return true;
}
