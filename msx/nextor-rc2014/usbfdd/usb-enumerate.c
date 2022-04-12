#include "usb-enumerate.h"
#include "hw.h"
#include "usb-enumerate-floppy.h"
#include "usb-enumerate-hub.h"

#include "debuggin.h"
#include "print.h"

struct __working;

typedef struct __working {
  usb_device_type          usb_device;
  const device_descriptor *desc;
  uint8_t *                next_storage_device_index;
  uint8_t                  config_index;
  uint8_t                  interface_count;
  uint8_t                  endpoint_count;

  const void *ptr;

  union {
    uint8_t           buffer[140];
    config_descriptor desc;
  } config;
} _working;

usb_error op_identify_class_driver(_working *const working) __z88dk_fastcall;
usb_error op_parse_endpoint(_working *const working) __z88dk_fastcall;

usb_device_type identify_class_driver(const interface_descriptor *const p) {

  if (p->bInterfaceClass == 8 && p->bInterfaceSubClass == 6 && p->bInterfaceProtocol == 80) {
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

usb_error get_config_descriptor(const device_descriptor *const desc, const uint8_t config_index, uint8_t *const buffer) {
  usb_error result;

  // printf("Config %d: ", config_index);
  CHECK(hw_get_config_descriptor(
      (config_descriptor *)buffer, config_index, desc->bMaxPacketSize0, sizeof(config_descriptor), 0));
  // logConfig((config_descriptor *)buffer);

  CHECK(hw_get_config_descriptor(
      (config_descriptor *)buffer, config_index, desc->bMaxPacketSize0, ((config_descriptor *)buffer)->wTotalLength, 0));

  return USB_ERR_OK;
}

usb_error op_interface_next(_working *const working) __z88dk_fastcall {
  if (--working->interface_count == 0)
    return USB_ERR_OK;

  return op_identify_class_driver(working);
}

usb_error op_capture_driver_2(_working *const working) __z88dk_fastcall {
  usb_error         result;
  _usb_state *const work_area = get_usb_work_area();

  storage_device_config *const storage_dev = &work_area->storage_device[*working->next_storage_device_index];
  device_config *const         dev_cfg     = &storage_dev->config;

  switch (working->usb_device) {
  case USB_IS_FLOPPY:
  case USB_IS_MASS_STORAGE:
    dev_cfg->max_packet_size = working->desc->bMaxPacketSize0;
    dev_cfg->value           = working->config.desc.bConfigurationvalue;
    dev_cfg->address         = 20 + *working->next_storage_device_index;
    CHECK(hw_set_address_and_configuration(dev_cfg));
    (*working->next_storage_device_index)++;
    break;

  case USB_IS_HUB:
    work_area->hub_config.max_packet_size = working->desc->bMaxPacketSize0;
    work_area->hub_config.value           = working->config.desc.bConfigurationvalue;
    configure_usb_hub(work_area, working->next_storage_device_index);
    break;
  }

  return op_interface_next(working);
}

usb_error op_endpoint_next(_working *const working) __z88dk_fastcall {
  if (--working->endpoint_count > 0) {
    working->ptr = ((endpoint_descriptor *)working->ptr) + 1;
    return op_parse_endpoint(working);
  }

  return op_capture_driver_2(working);
}

usb_error op_parse_endpoint(_working *const working) __z88dk_fastcall {
  _usb_state *const work_area = get_usb_work_area();

  const endpoint_descriptor *endpoint = (endpoint_descriptor *)working->ptr;

  // printf("EndP: ");
  // logEndPointDescription(endpoint);

  switch (working->usb_device) {
  case USB_IS_FLOPPY:
  case USB_IS_MASS_STORAGE:
    parse_endpoint_floppy(work_area, endpoint);
    break;

  case USB_IS_HUB:
    parse_endpoint_hub(work_area, endpoint);
  }

  return op_endpoint_next(working);
}

usb_error op_capture_driver_interface(_working *const working) __z88dk_fastcall {
  _usb_state *const           work_area        = get_usb_work_area();
  const interface_descriptor *interface        = (interface_descriptor *)working->ptr;
  const uint8_t               bInterfaceNumber = interface->bInterfaceNumber;

  // printf("Inter: %d ", working->usb_device);
  // logInterface(interface);

  // if (work_area->xusb_device & working->usb_device)
  //   *usb_device = 0;

  work_area->xusb_device |= working->usb_device;

  switch (working->usb_device) {
  case USB_IS_FLOPPY:
  case USB_IS_MASS_STORAGE:
    work_area->storage_device[0].type                    = working->usb_device;
    work_area->storage_device[0].config.interface_number = bInterfaceNumber;
    break;

  case USB_IS_HUB:
    work_area->hub_config.interface_number = bInterfaceNumber;
  }

  working->ptr            = interface + 1;
  working->endpoint_count = interface->bNumEndpoints;
  return op_parse_endpoint(working);
}

usb_error op_identify_class_driver(_working *const working) __z88dk_fastcall {
  working->usb_device = identify_class_driver((interface_descriptor *)working->ptr);
  return op_capture_driver_interface(working);
}

usb_error op_get_config_descriptor(_working *const working) __z88dk_fastcall {
  usb_error result;

  CHECK(get_config_descriptor(working->desc, working->config_index, working->config.buffer));

  working->ptr             = (working->config.buffer + sizeof(config_descriptor));
  working->interface_count = working->config.desc.bNumInterfaces;

  return op_identify_class_driver(working);
}

usb_error parse_config(_usb_state *const              work_area,
                       const device_descriptor *const desc,
                       const uint8_t                  config_index,
                       uint8_t *const                 next_storage_device_index) {
  (void)work_area;
  _working working;
  working.usb_device                = 0;
  working.desc                      = desc;
  working.config_index              = config_index;
  working.next_storage_device_index = next_storage_device_index;

  return op_get_config_descriptor(&working);
}

usb_error read_all_configs(uint8_t *const next_storage_device_index) {
  _usb_state *const work_area = get_usb_work_area();
  device_descriptor desc;
  uint8_t           result;

  CHECK(hw_get_description(0, 64, &desc), x_printf("ErrX %02x\r\n", result));

  // printf("Desc: ");
  // logDevice(&desc);

  for (uint8_t config_index = 0; config_index < desc.bNumConfigurations; config_index++)
    CHECK(parse_config(work_area, &desc, config_index, next_storage_device_index));

  return USB_ERR_OK;
}

usb_error enumerate_all_devices() {
  uint8_t next_storage_device_index = 0;

  const usb_error result = read_all_configs(&next_storage_device_index);
  printf("dev-count: %d\r\n", next_storage_device_index);
  return result;
}