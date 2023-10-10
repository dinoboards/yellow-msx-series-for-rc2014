#include "arguments.h"
#include "reporters.h"
#include "rotator.h"
#include "ufi_reporters.h"
#include <class_ufi.h>
#include <delay.h>
#include <extbio.h>
#include <extbio_rc2014.h>
#include <fusion_ext.h>
#include <io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint8_t report_all_devices(const uint8_t last_device_address) __sdcccall(1) {
  uint8_t   buffer[151];
  usb_error result      = 0;
  int       file_handle = 0;

  printf("Enumerating %d devices\r\n", last_device_address);

  rotate_init();

  memset(buffer, 0, sizeof(buffer));
  file_handle = Open((char *)report_file_name, IO_CREAT);

  device_descriptor my_device_descriptor;
  memset(&my_device_descriptor, 0, sizeof(device_descriptor));

  fusion_fprintf(file_handle, "Device Count: %d\r\n", last_device_address);

  for (uint8_t device_address = 1; device_address <= last_device_address; device_address++) {
    erase_line();
    printf("\rdevice %d  ", device_address);
    rotate();
    usb_error result = usbtrn_get_descriptor2(&my_device_descriptor, device_address);
    if (result) {
      printf("usbtrn_get_descriptor2 failed %d\r\n", result);
      continue;
    }

    report_device_descriptor(&my_device_descriptor, file_handle);

    for (uint8_t config_index = 0; config_index < my_device_descriptor.bNumConfigurations; config_index++) {
      memset(buffer, 0, sizeof(buffer));
      result = usbtrn_get_full_config_descriptor(config_index, device_address, my_device_descriptor.bMaxPacketSize0, 150, buffer);
      if (result) {
        printf("usbtrn_get_full_config_descriptor failed %d\r\n", result);
        continue;
      }

      const config_descriptor *const config = (config_descriptor *)buffer;
      report_device_configuration(config, file_handle);

      for (uint8_t interface_index = 0; interface_index < config->bNumInterfaces; interface_index++) {
        const interface_descriptor *const interface =
            (interface_descriptor *)(buffer + sizeof(config_descriptor) + interface_index * sizeof(interface_descriptor));
        report_device_interface(interface, file_handle);

        for (uint8_t endpoint_index = 0; endpoint_index < interface->bNumEndpoints; endpoint_index++) {
          const endpoint_descriptor *const endpoint = (endpoint_descriptor *)&interface[1];
          report_device_endpoint(&endpoint[endpoint_index], file_handle);
        }
      }
    }
  }

  Close(file_handle);
  rotate_stop();
  erase_line();

  return 0;
}

usb_error find_floppy_interface(const uint8_t address, device_config_interface *const floppy_interface) {
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

uint8_t report_floppies(const uint8_t last_device_address) __sdcccall(1) {
  device_config_interface floppy_interface;
  memset(&floppy_interface, 0, sizeof(device_config_interface));

  for (uint8_t address = 1; address <= last_device_address; address++) {

    usb_error result = find_floppy_interface(address, &floppy_interface);

    if (result && result != 255) {
      printf("USB error %02x\r\n", result);
      return result;
    }

    if (result == 0)
      printf("Floppy @ address %d, config index %d and interface %d\r\n", floppy_interface.address, floppy_interface.config_index,
             floppy_interface.interface_index);
  }

  return 0;
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

uint8_t retrieve_floppy_formats(device_config *const storage_device, ufi_format_capacities_response *const r) {
  usb_error result;

  // printf("Device Configuration for Floppy:\r\n");
  // printf(" max_packet_size: %d", storage_device.max_packet_size);
  // printf(" address: %d", storage_device.address);
  // printf(" interface_number: %d\r\n", storage_device.interface_number);

  // for (uint8_t i = 0; i < 3; i++) {
  //   printf("  Endpoint\r\n");
  //   printf("    number: %d\r\n", storage_device.endpoints[i].number);
  //   printf("    toggle: %d\r\n", storage_device.endpoints[i].toggle);
  //   printf("    max_pkt_size: %d\r\n", storage_device.endpoints[i].max_packet_sizex);
  // }

  wait_for_device_ready(storage_device, 2500);
  //   printf("READY\r\n");
  // }
  // else
  //   printf("not ready\r\n");

  ufi_inquiry_response response;
  memset(&response, 0, sizeof(ufi_inquiry_response));
  ufi_inquiry(storage_device, &response);
  // printf("USB inquiry: %d\r\n", result);

  wait_for_device_ready(storage_device, 1000);
  // if (wait_for_device_ready(&storage_device, 1000) == 0) {
  //   printf("READY\r\n");
  // }
  // else
  //   printf("not ready\r\n");

  result = ufi_read_format_capacities(storage_device, r);
  if (result) {
    printf("USB Error reading floppy's format capacities: %d\r\n", result);
    return result;
  }

  return 0;
}

uint8_t write_sector(device_config *const storage_device, const uint16_t sector_number, uint8_t *buffer) {
  usb_error            result;
  ufi_interrupt_status sense_codes;

  if (wait_for_device_ready(storage_device, 2000) != 0)
    return 255;

  memset(&sense_codes, 0, sizeof(sense_codes));
  if ((result = ufi_read_write_sector(storage_device, true, sector_number, 1, buffer, (uint8_t *)&sense_codes)) != USB_ERR_OK)
    return result;

  ufi_request_sense_response response;
  memset(&response, 0, sizeof(response));

  if ((result = ufi_request_sense(storage_device, &response)) != USB_ERR_OK)
    return result;

  const uint8_t asc       = response.asc;
  const uint8_t ascq      = response.ascq;
  const uint8_t sense_key = response.sense_key;

  if (sense_key != 0)
    return 255;

  return 0;
}

uint8_t read_sector(device_config *const storage_device, const uint16_t sector_number, uint8_t *buffer) {

  if (wait_for_device_ready(storage_device, 2000) != 0)
    return 255;

  usb_error            result;
  ufi_interrupt_status sense_codes;

  memset(&sense_codes, 0, sizeof(sense_codes));

  if ((result = ufi_read_write_sector(storage_device, false, sector_number, 1, buffer, (uint8_t *)&sense_codes)) != USB_ERR_OK)
    return result;

  ufi_request_sense_response response;
  memset(&response, 0, sizeof(response));

  if ((result = ufi_request_sense(storage_device, &response)) != USB_ERR_OK)
    return result;

  const uint8_t asc       = response.asc;
  const uint8_t ascq      = response.ascq;
  const uint8_t sense_key = response.sense_key;

  if (sense_key != 0)
    return 255;

  return 0;
}

uint8_t format_floppy(void) __sdcccall(1) {
  usb_error result;

  ufi_format_capacities_response r;
  memset(&r, 0, sizeof(r));

  device_config storage_device;
  memset(&storage_device, 0, sizeof(storage_device));

  result = construct_device_config(&storage_device);
  if (result)
    return result;

  result = retrieve_floppy_formats(&storage_device, &r);
  if (result)
    return result;

  log_ufi_format_capacities_response(&r);

  for (uint8_t track_number = 0; track_number < 80; track_number++)
    for (uint8_t side = 0; side < 2; side++) {
      wait_for_device_ready(&storage_device, 2000);

      printf("format track: %d, side: %d\r", track_number, side);
      result = ufi_format(&storage_device, side, track_number, &r.descriptors[0]);

      if (result) {
        printf("\rFormat failure @ %d, %d: %d\r\n", track_number, side, result);
        return result;
      }

      ufi_request_sense_response response;
      memset(&response, 0, sizeof(response));

      if ((result = ufi_request_sense(&storage_device, &response)) != USB_ERR_OK)
        return result;

      const uint8_t asc       = response.asc;
      const uint8_t ascq      = response.ascq;
      const uint8_t sense_key = response.sense_key;

      if (sense_key != 0) {
        printf("\rFormat failure @ %d, %d, %d\r\n", sense_key, asc, ascq);
        return 255;
      }

      // uint8_t buffer[512];

      // const uint16_t sector_start = ((track_number*2)+side)*18;

      // for(uint16_t sector_number = sector_start; sector_number < sector_start+18; sector_number++) {
      //   printf("\r\n%d: ", sector_number);
      //   if (wait_for_device_ready(&storage_device, 1000) != 0) {
      //     printf("not ready\r\n");
      //     return 255;
      //   }

      //   result = ufi_read_write_sector(&storage_device, true, sector_number, 1, buffer, NULL);
      //   if (result) {
      //     printf("write failed %d\r\n", result);
      //   }

      //   result = read_sector(&storage_device, sector_number, buffer);
      //   if (result) {
      //     printf("read failed %d, trying again...\r\n", result);
      //     result = read_sector(&storage_device, sector_number, buffer);
      //     if (result) {
      //       printf("read failed again %d\r\n", result);
      //     }
      //   }
      // }
    }

  printf("\r\n");

  return 0;
}

uint8_t check_floppy_sectors(void) __sdcccall(1) {
  usb_error result;
  uint8_t   buffer[512];

  ufi_format_capacities_response r;
  memset(&r, 0, sizeof(r));

  device_config storage_device;
  memset(&storage_device, 0, sizeof(storage_device));

  result = construct_device_config(&storage_device);
  if (result)
    return result;

  for (uint16_t sector_number = 0; sector_number < 2880; sector_number++) {
    printf("\r\n%d: ", sector_number);
    if (wait_for_device_ready(&storage_device, 1000) != 0) {
      printf("not ready\r\n");
      return 255;
    }

    result = ufi_read_write_sector(&storage_device, false, sector_number, 1, buffer, NULL);
    if (result) {
      printf("read failed %d\r\n", result);
      // return result;
    }
  }

  printf("\r\n");

  return 0;
}

uint8_t main(const int argc, const char *argv[]) {
  process_cli_arguments(argc, argv);

  uint16_t      hl = extbio_usb_get_present();
  const uint8_t h  = hl >> 8;
  const uint8_t l  = hl & 0xFF;

  if (!h) {
    printf("CH376 not detected\r\n");
    return 255;
  }

  switch (subcommand) {
  case cmd_none:
    break;

  case cmd_format_floppy:
    return format_floppy();

  case cmd_check_floppy_sectors:
    return check_floppy_sectors();

  case cmd_report_to_file:
    report_all_devices(l);
    printf("usb device details saved to file %s\r\n", report_file_name);
    break;

  case cmd_report_floppies:
    return report_floppies(l);
  }

  // if (h) {
  //   report_all_devices(l);
  //   // find first floppy drive
  // }

  // ufi_format_capacities_response r;
  // memset(&r, 0, sizeof(r));

  // device_config storage_device;

  // if (wait_for_device_ready(&storage_device, 2500) == 0) {
  //   hl = ufi_read_format_capacities(&storage_device, &r);
  //   printf("ufi_read_format_capacities: %d,", hl);
  //   log_ufi_format_capacities_response(&r);
  // }

  return 0;
}
