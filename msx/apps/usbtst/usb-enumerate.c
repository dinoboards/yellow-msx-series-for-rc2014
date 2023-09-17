#include "usb-enumerate.h"
#include "hw.h"
#include "usb-enumerate-floppy.h"
#include "usb-enumerate-hub.h"
#include <string.h>

#include "debuggin.h"
#include "print.h"

usb_error op_identify_class_driver(_working *const working) __z88dk_fastcall;
usb_error op_parse_endpoint(_working *const working) __z88dk_fastcall;

usb_device_type identify_class_driver(_working *const working) {
  const interface_descriptor *const p = (const interface_descriptor *)working->ptr;
  if (p->bInterfaceClass == 2) {
    return USB_IS_CDC;
  }

  if (p->bInterfaceClass == 8 && (p->bInterfaceSubClass == 6 || p->bInterfaceSubClass == 5) && p->bInterfaceProtocol == 80) {
    return USB_IS_MASS_STORAGE;
  }

  if (p->bInterfaceClass == 8 && p->bInterfaceSubClass == 4 && p->bInterfaceProtocol == 0) {
    return USB_IS_FLOPPY;
  }

  if (p->bInterfaceClass == 9 && p->bInterfaceSubClass == 0 && p->bInterfaceProtocol == 0) {
    return USB_IS_HUB;
  }

  return 0;
}

usb_error get_config_descriptor(const uint8_t  config_index,
                                const uint8_t  device_address,
                                const uint8_t  max_packet_size,
                                uint8_t *const buffer) {
  usb_error result;

  printf("GET CONFIG %d, %d, %d\r\n", config_index, device_address, max_packet_size);

  CHECK(hw_get_config_descriptor((config_descriptor *)buffer, config_index, sizeof(config_descriptor), device_address,
                                 max_packet_size));

  // if wTotalLength > MAX_CONFIG_SIZE bad things might happen
  CHECK(hw_get_config_descriptor((config_descriptor *)buffer, config_index, ((config_descriptor *)buffer)->wTotalLength,
                                 device_address, max_packet_size));
  logConfig((config_descriptor *)buffer);

  return USB_ERR_OK;
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

  printf("EndP: ");
  logEndPointDescription(endpoint);

  storage_device_config *const storage_dev = &work_area->storage_device[work_area->next_storage_device_index];

  switch (working->usb_device) {
  case USB_IS_FLOPPY:
  case USB_IS_MASS_STORAGE:
    parse_endpoint_storage(storage_dev, endpoint);
    break;

  case USB_IS_HUB:
    parse_endpoint_hub(endpoint);
  }

  return op_endpoint_next(working);
}

usb_error op_capture_driver_interface(_working *const working) __z88dk_fastcall {
  usb_error                         result;
  _usb_state *const                 work_area = get_usb_work_area();
  const interface_descriptor *const interface = (interface_descriptor *)working->ptr;

  printf("Intf ");
  logInterface(interface);

  working->ptr += interface->bLength;
  working->endpoint_count = interface->bNumEndpoints;

  switch (working->usb_device) {
  case USB_IS_FLOPPY:
  case USB_IS_MASS_STORAGE: {
    work_area->next_storage_device_index++;
    storage_device_config *const storage_dev = &work_area->storage_device[work_area->next_storage_device_index];
    device_config *const         dev_cfg     = &storage_dev->config;

    dev_cfg->max_packet_size  = working->desc.bMaxPacketSize0;
    dev_cfg->value            = working->config.desc.bConfigurationvalue;
    dev_cfg->address          = working->state->next_device_address;
    dev_cfg->interface_number = interface->bInterfaceNumber;
    storage_dev->type         = working->usb_device;
    CHECK(hw_set_configuration(dev_cfg));
    break;
  }

  case USB_IS_CDC: {
    printf("config ethernet adapter\r\n");

    work_area->cdc_config.interface_number = interface->bInterfaceNumber;
    work_area->cdc_config.max_packet_size  = working->desc.bMaxPacketSize0;
    work_area->cdc_config.value            = working->config.desc.bConfigurationvalue;
    work_area->cdc_config.address          = working->state->next_device_address;
    CHECK(hw_set_configuration(&work_area->cdc_config));
    break;
  }

  case USB_IS_HUB: {
    work_area->hub_config.interface_number = interface->bInterfaceNumber;
    work_area->hub_config.max_packet_size  = working->desc.bMaxPacketSize0;
    work_area->hub_config.value            = working->config.desc.bConfigurationvalue;
    work_area->hub_config.address          = working->state->next_device_address;
    CHECK(configure_usb_hub(working));
    break;
  }

  default:
    working->state->next_device_address;
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

usb_error op_get_config_descriptor(_working *const working) __z88dk_fastcall {
  usb_error result;

  memset(working->config.buffer, 0, MAX_CONFIG_SIZE);

  const uint8_t max_packet_size = working->desc.bMaxPacketSize0;

  CHECK(get_config_descriptor(working->config_index, working->state->next_device_address, max_packet_size, working->config.buffer));

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

  result = hw_get_description(&working.desc);

  CHECK(result);

  logDevice(&working.desc);

  state->next_device_address++;
  const uint8_t dev_address = state->next_device_address;
  CHECK(hw_set_address(dev_address));

  for (uint8_t config_index = 0; config_index < working.desc.bNumConfigurations; config_index++) {
    working.config_index = config_index;

    CHECK(op_get_config_descriptor(&working));
  }

  printf("read-all-ok\r\n");
  return USB_ERR_OK;
}

usb_error enumerate_all_devices() {
  _usb_state *const work_area          = get_usb_work_area();
  work_area->next_storage_device_index = (uint8_t)-1;
  enumeration_state state;
  memset(&state, 0, sizeof(enumeration_state));
  state.next_device_address = 1;

  usb_error result = read_all_configs(&state);

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
