#include "usb-enumerate.h"
#include "hw.h"
#include "usb-enumerate-floppy.h"
#include "usb-enumerate-hub.h"

#include "debuggin.h"
#include "print.h"

usb_device_type identify_class_driver(_usb_state *const work_area, const interface_descriptor *const p) {
  usb_device_type *const dev = &work_area->usb_device;

  if (p->bInterfaceClass == 8 && p->bInterfaceSubClass == 6 && p->bInterfaceProtocol == 80 &&
      !(*dev & USB_IS_MASS_STORAGE)) {
    *dev |= USB_IS_MASS_STORAGE;
    return USB_IS_MASS_STORAGE;
  }

  if (p->bInterfaceClass == 8 && p->bInterfaceSubClass == 4 && p->bInterfaceProtocol == 0 && !(*dev & USB_IS_FLOPPY)) {
    *dev |= USB_IS_FLOPPY;
    return USB_IS_FLOPPY;
  }

  if (p->bInterfaceClass == 9 && p->bInterfaceSubClass == 0 && p->bInterfaceProtocol == 0 && !(*dev & USB_IS_HUB)) {
    *dev |= USB_IS_HUB;
    return USB_IS_HUB;
  }

  return 0;
}

const interface_descriptor *
parse_interface(_usb_state *const work_area, const interface_descriptor *const p, usb_device_type *const usb_device) {
  // logInterface(p);

  work_area->interface_number = p->bInterfaceNumber;

  *usb_device = identify_class_driver(work_area, p);

  const endpoint_descriptor *pEndpoint = (const endpoint_descriptor *)(p + 1);

  for (uint8_t endpoint_index = p->bNumEndpoints; endpoint_index != 0; endpoint_index--) {
    // logEndPointDescription(pEndpoint);

    switch (*usb_device) {
    case USB_IS_FLOPPY:
      parse_endpoint_floppy(work_area, pEndpoint);
      break;

    case USB_IS_HUB:
      parse_endpoint_hub(work_area, pEndpoint);
    }
    pEndpoint++;
  }

  return (interface_descriptor *)pEndpoint;
}

usb_error parse_config(_usb_state *const work_area, const device_descriptor *const desc, const uint8_t config_index) {
  uint8_t                  result;
  uint8_t                  buffer[140];
  config_descriptor *const config_desc = (config_descriptor *)buffer;

  // printf("Config %d: ", config_index);

  result = hw_get_config_descriptor(config_desc, config_index, desc->bMaxPacketSize0, sizeof(config_descriptor), 0);
  if (result != USB_ERR_OK) {
    // yprintf(15, "X1 (%d)", result);
    return result;
  }
  // logConfig(config_desc);

  result = hw_get_config_descriptor(config_desc, config_index, desc->bMaxPacketSize0, config_desc->wTotalLength, 0);
  if (result != USB_ERR_OK) {
    // yprintf(15, "X2 (%d)", result);
    return result;
  }

  usb_device_type usb_device = 0;

  const interface_descriptor *p = (const interface_descriptor *)(buffer + sizeof(config_descriptor));
  for (uint8_t interface_index = 0; interface_index < config_desc->bNumInterfaces; interface_index++) {
    // printf("Interf %d: ", interface_index);
    p = parse_interface(work_area, p, &usb_device);

    switch (usb_device) {
    case USB_IS_FLOPPY:
      work_area->floppy_config.max_packet_size = desc->bMaxPacketSize0;
      work_area->floppy_config.value           = config_desc->bConfigurationvalue;
      hw_set_address_and_configuration(DEVICE_ADDRESS_FLOPPY, &work_area->floppy_config);
      break;

    case USB_IS_HUB:
      work_area->hub_config.max_packet_size = desc->bMaxPacketSize0;
      work_area->hub_config.value           = config_desc->bConfigurationvalue;
      configure_usb_hub(work_area);
      break;
    }
  }

  return USB_ERR_OK;
}

usb_error read_all_configs() {
  _usb_state *const work_area = get_usb_work_area();
  device_descriptor desc;
  uint8_t           result;

  CHECK(hw_get_description(0, 64, &desc), x_printf("ErrX %02x\r\n", result));

  // printf("Desc: ");
  // logDevice(&desc);

  for (uint8_t config_index = 0; config_index < desc.bNumConfigurations; config_index++) {
    if ((result = parse_config(work_area, &desc, config_index)) != USB_ERR_OK)
      return result;
  }

  return USB_ERR_OK;
}