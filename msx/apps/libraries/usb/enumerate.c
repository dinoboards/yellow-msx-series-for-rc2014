#include "enumerate.h"
#include "enumerate_hub.h"
#include "enumerate_storage.h"
#include "protocol.h"
#include <string.h>

void parse_endpoint_printer(const endpoint_descriptor const *pEndpoint) __z88dk_fastcall {
  _usb_state *const work_area = get_usb_work_area();

  endpoint_param *const ep = &work_area->printer_config.endpoints[0];
  ep->number               = pEndpoint->bEndpointAddress;
  ep->toggle               = 0;
  ep->max_packet_sizex     = calc_max_packet_sizex(pEndpoint->wMaxPacketSize);
}

usb_error op_identify_class_driver(_working *const working) __z88dk_fastcall;
usb_error op_parse_endpoint(_working *const working) __z88dk_fastcall;

usb_device_type identify_class_driver(_working *const working) {
  const interface_descriptor *const p = (const interface_descriptor *)working->ptr;
  if (p->bInterfaceClass == 2)
    return USB_IS_CDC;

  if (p->bInterfaceClass == 7)
    return USB_IS_PRINTER;

  if (p->bInterfaceClass == 8 && (p->bInterfaceSubClass == 6 || p->bInterfaceSubClass == 5) && p->bInterfaceProtocol == 80)
    return USB_IS_MASS_STORAGE;

  if (p->bInterfaceClass == 8 && p->bInterfaceSubClass == 4 && p->bInterfaceProtocol == 0)
    return USB_IS_FLOPPY;

  if (p->bInterfaceClass == 9 && p->bInterfaceSubClass == 0 && p->bInterfaceProtocol == 0)
    return USB_IS_HUB;

  return 0;
}

usb_error op_interface_next(_working *const working) __z88dk_fastcall {
  if (--working->interface_count == 0)
    return USB_ERR_OK;

  return op_identify_class_driver(working);
}

usb_error op_endpoint_next(_working *const working) __z88dk_fastcall {
  if (--working->endpoint_count > 0) {
    working->ptr += ((endpoint_descriptor *)working->ptr)->bLength;
    return op_parse_endpoint(working);
  }

  return op_interface_next(working);
}

usb_error op_parse_endpoint(_working *const working) __z88dk_fastcall {
  _usb_state *const work_area = get_usb_work_area();

  const endpoint_descriptor *endpoint = (endpoint_descriptor *)working->ptr;

  switch (working->usb_device) {
  case USB_IS_FLOPPY:
  case USB_IS_MASS_STORAGE: {
    device_config *const storage_dev = &work_area->storage_device[working->state->next_storage_device_index];
    parse_endpoint_storage(storage_dev, endpoint);
    break;
  }

  case USB_IS_PRINTER: {
    parse_endpoint_printer(endpoint);
    break;
  }

    // case USB_IS_HUB: {
    //   break;
    // }

    // case USB_IS_CDC: {
    //   // print_string("TODO parse CDC\r\n");
    //   // print_hex(((uint16_t)endpoint) >> 8);
    //   // print_hex((uint16_t)endpoint);
    //   // print_string(")\r\n");
    //   break;
    // }
  }

  return op_endpoint_next(working);
}

usb_error
configure_device(const _working *const working, const interface_descriptor *const interface, device_config *const dev_cfg) {
  usb_error result;

  dev_cfg->interface_number = interface->bInterfaceNumber;
  dev_cfg->max_packet_size  = working->desc.bMaxPacketSize0;
  dev_cfg->address          = working->current_device_address;
  dev_cfg->type             = working->usb_device;

  RETURN_CHECK(usbtrn_set_configuration(dev_cfg->address, dev_cfg->max_packet_size, working->config.desc.bConfigurationvalue));
}

usb_error op_capture_hub_driver_interface(_working *const working) __sdcccall(1) {
  const interface_descriptor *const interface = (interface_descriptor *)working->ptr;

  usb_error         result;
  device_config_hub hub_config;
  working->hub_config = &hub_config;

  hub_config.type = USB_IS_HUB;
  CHECK(configure_device(working, interface, (device_config *const)&hub_config));
  RETURN_CHECK(configure_usb_hub(working));
}

usb_error op_capture_driver_interface(_working *const working) __z88dk_fastcall {
  usb_error                         result;
  _usb_state *const                 work_area = get_usb_work_area();
  const interface_descriptor *const interface = (interface_descriptor *)working->ptr;

  working->ptr += interface->bLength;
  working->endpoint_count = interface->bNumEndpoints;

  switch (working->usb_device) {
  case USB_IS_FLOPPY:
  case USB_IS_MASS_STORAGE: {
    working->state->next_storage_device_index++;
    device_config *const storage_dev = &work_area->storage_device[working->state->next_storage_device_index];
    device_config *const dev_cfg     = storage_dev;
    CHECK(configure_device(working, interface, dev_cfg));
    break;
  }

  case USB_IS_PRINTER: {
    work_area->printer_config.type = USB_IS_PRINTER;
    CHECK(configure_device(working, interface, (device_config *const)&work_area->printer_config));
    break;
  }

  case USB_IS_CDC: {
    work_area->cdc_config.type = USB_IS_CDC;
    CHECK(configure_device(working, interface, &work_area->cdc_config));
    break;
  }

  case USB_IS_HUB: {
    CHECK(op_capture_hub_driver_interface(working))
    break;
  }
  }

  CHECK(op_parse_endpoint(working));

  return result;
}

usb_error op_identify_class_driver(_working *const working) __z88dk_fastcall {
  usb_error                         result;
  const interface_descriptor *const ptr = (const interface_descriptor *)working->ptr;

  working->usb_device = ptr->bLength > 5 ? identify_class_driver(working) : 0;

  CHECK(op_capture_driver_interface(working));

  return result;
}

usb_error op_get_config_descriptor(_working *const working) __sdcccall(1) {
  usb_error result;

  memset(working->config.buffer, 0, MAX_CONFIG_SIZE);

  const uint8_t max_packet_size = working->desc.bMaxPacketSize0;

  CHECK(usbtrn_get_full_config_descriptor(working->config_index, working->current_device_address, max_packet_size, MAX_CONFIG_SIZE,
                                          working->config.buffer));

  working->ptr             = (working->config.buffer + sizeof(config_descriptor));
  working->interface_count = working->config.desc.bNumInterfaces;

  CHECK(op_identify_class_driver(working));

  return result;
}

usb_error read_all_configs(enumeration_state *const state) {
  uint8_t           result;
  _usb_state *const work_area = get_usb_work_area();

  _working working;
  memset(&working, 0, sizeof(_working));
  working.state = state;

  CHECK(usbtrn_get_descriptor(&working.desc));

  state->next_device_address++;
  working.current_device_address = state->next_device_address;
  CHECK(usbtrn_set_address(working.current_device_address));

  for (uint8_t config_index = 0; config_index < working.desc.bNumConfigurations; config_index++) {
    working.config_index = config_index;

    CHECK(op_get_config_descriptor(&working));
  }

  return USB_ERR_OK;
}

usb_error enumerate_all_devices(void) {
  _usb_state *const work_area = get_usb_work_area();
  enumeration_state state;
  memset(&state, 0, sizeof(enumeration_state));
  state.next_device_address       = 0;
  state.next_storage_device_index = -1;

  usb_error result = read_all_configs(&state);

  work_area->count_of_detected_usb_devices = state.next_device_address;

  CHECK(result);

  return result;
}

/*
  enumerate_all_devices
    -> read_all_configs
      -> parse_config
        -> op_get_config_descriptor
          -> op_identify_class_driver
            -> op_capture_driver_interface (increment index)
              -> op_parse_endpoint
                -> parse_endpoint_storage
                -> parse_endpoint_hub
                -> op_endpoint_next
                  -> op_parse_endpoint -^ (install driver endpoint)
                  -> op_interface_next
                    -> return
                    -> op_identify_class_driver -^


*/
