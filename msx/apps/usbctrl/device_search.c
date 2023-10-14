#include "device_search.h"
#include <protocol.h>

usb_error find_next_floppy_interface(const uint8_t address, device_config_interface *const floppy_interface) __sdcccall(1) {
  uint8_t buffer[151];

  device_descriptor my_device_descriptor;
  memset(&my_device_descriptor, 0, sizeof(device_descriptor));

  usb_error result = usbtrn_get_descriptor2(&my_device_descriptor, address);
  if (result) {
    printf("usbtrn_get_descriptor2 failed %d\r\n", result);
    return result;
  }

  for (uint8_t config_index = 0; config_index < my_device_descriptor.bNumConfigurations; config_index++) {
    memset(buffer, 0, sizeof(buffer));
    result = usbtrn_get_full_config_descriptor(config_index, address, my_device_descriptor.bMaxPacketSize0, 150, buffer);
    if (result) {
      printf("usbtrn_get_full_config_descriptor failed %d\r\n", result);
      return result;
    }

    const config_descriptor *const config = (config_descriptor *)buffer;

    for (uint8_t interface_index = 0; interface_index < config->bNumInterfaces; interface_index++) {
      const interface_descriptor *const interface =
          (interface_descriptor *)(buffer + sizeof(config_descriptor) + interface_index * sizeof(interface_descriptor));

      if (interface->bInterfaceClass == 8 && interface->bInterfaceSubClass == 4 && interface->bInterfaceProtocol == 0) {
        floppy_interface->address         = address;
        floppy_interface->config_index    = config_index;
        floppy_interface->interface_index = interface_index;
        return USB_ERR_OK;
      }
    }
  }

  return 255;
}

uint8_t construct_device_config(device_config *const storage_device) __sdcccall(1) {
  const uint8_t device_address  = device_interface.address;
  const uint8_t config_index    = device_interface.config_index;
  const uint8_t interface_index = device_interface.interface_index;

  usb_error result;
  uint8_t   buffer[151];
  memset(buffer, 0, sizeof(buffer));

  device_descriptor my_device_descriptor;
  memset(&my_device_descriptor, 0, sizeof(device_descriptor));

  result = usbtrn_get_descriptor2(&my_device_descriptor, device_address);
  if (result) {
    printf("usbtrn_get_descriptor2 failed %d\r\n", result);
    return result;
  }

  result = usbtrn_get_full_config_descriptor(config_index, device_address, my_device_descriptor.bMaxPacketSize0, 150, buffer);
  if (result) {
    printf("usbtrn_get_full_config_descriptor failed %d\r\n", result);
    return result;
  }

  const config_descriptor *const    config = (config_descriptor *)buffer;
  const interface_descriptor *const interface =
      (interface_descriptor *)(buffer + sizeof(config_descriptor) + interface_index * sizeof(interface_descriptor));

  storage_device->max_packet_size  = my_device_descriptor.bMaxPacketSize0;
  storage_device->address          = device_address;
  storage_device->interface_number = interface->bInterfaceNumber;
  storage_device->type             = USB_IS_FLOPPY;

  for (uint8_t endpoint_index = 0; endpoint_index < interface->bNumEndpoints; endpoint_index++) {
    const endpoint_descriptor *const endpoints = (endpoint_descriptor *)&interface[1];

    const uint8_t         x   = calc_max_packet_sizex(endpoints[endpoint_index].wMaxPacketSize);
    endpoint_param *const eps = storage_device->endpoints;
    endpoint_param       *ep;

    if (endpoints[endpoint_index].bmAttributes & 0x01) { // 3 -> Interrupt
      if (!(endpoints[endpoint_index].bEndpointAddress & 0x80))
        continue;

      ep = &eps[ENDPOINT_INTERRUPT_IN];

    } else {
      ep = (endpoints[endpoint_index].bEndpointAddress & 0x80) ? &eps[ENDPOINT_BULK_IN] : &eps[ENDPOINT_BULK_OUT];
    }

    ep->number           = endpoints[endpoint_index].bEndpointAddress & 0x07;
    ep->toggle           = 0;
    ep->max_packet_sizex = x;
  }

  return 0;
}

usb_error retrieve_floppy_formats(device_config *const storage_device, ufi_format_capacities_response *const r) __sdcccall(1) {
  usb_error result;

  wait_for_device_ready(storage_device, 2500);

  ufi_inquiry_response response;
  memset(&response, 0, sizeof(ufi_inquiry_response));
  result = ufi_inquiry(storage_device, &response);
  if (result) {
    printf("USB Error reading floppy's status: %d\r\n", result);
    return result;
  }

  wait_for_device_ready(storage_device, 1000);

  result = ufi_read_format_capacities(storage_device, r);
  if (result) {
    printf("USB Error reading floppy's format capacities: %d\r\n", result);
    return result;
  }

  return 0;
}
