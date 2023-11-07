#include "find_device.h"
#include <msxbios/extbio_rc2014.h>
#include <string.h>

uint8_t construct_device_config(const uint8_t        device_address,
                                const uint8_t        config_index,
                                const uint8_t        interface_index,
                                device_config *const storage_device) __sdcccall(1) {
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

usb_error match_device(const uint8_t address, interface_match_fn fn, device_config *const device) __sdcccall(1) {
  uint8_t buffer[151];

  device_descriptor my_device_descriptor;
  memset(&my_device_descriptor, 0, sizeof(device_descriptor));

  usb_error result = usbtrn_get_descriptor2(&my_device_descriptor, address);
  __asm__("EI");
  if (result) {
    printf("usbtrn_get_descriptor2 failed %d\r\n", result);
    return result;
  }

  for (uint8_t config_index = 0; config_index < my_device_descriptor.bNumConfigurations; config_index++) {
    memset(buffer, 0, sizeof(buffer));
    result = usbtrn_get_full_config_descriptor(config_index, address, my_device_descriptor.bMaxPacketSize0, 150, buffer);
    __asm__("EI");
    if (result) {
      printf("usbtrn_get_full_config_descriptor failed %d\r\n", result);
      return result;
    }

    const config_descriptor *const config = (config_descriptor *)buffer;

    for (uint8_t interface_index = 0; interface_index < config->bNumInterfaces; interface_index++) {
      const interface_descriptor *const interface =
          (interface_descriptor *)(buffer + sizeof(config_descriptor) + interface_index * sizeof(interface_descriptor));

      if (fn(&my_device_descriptor, interface)) {
        return construct_device_config(address, config_index, interface_index, device);
      }
    }
  }

  return 255;
}

usb_error find_device(interface_match_fn fn, device_config *const config) __sdcccall(1) {
  uint16_t      hl                  = extbio_usb_get_present();
  const uint8_t h                   = hl >> 8;
  const uint8_t last_device_address = hl & 0xFF;

  if (!h)
    return 255;

  for (uint8_t address = 1; address <= last_device_address; address++) {
    const usb_error result = match_device(address, fn, config);

    if (result == 0)
      return 0;

    if (result == 255)
      continue;

    return result;
  }

  return 255;
}
