#include <extbio_rc2014.h>
#include <msx_fusion.h>
#include <stdio.h>
#include <string.h>
#include <sysload/sys.h>
#include <sysload/sysload.h>
#include <system_vars.h>
#include <usb/class_printer.h>

typedef struct {
  uint8_t address;
  uint8_t config_index;
  uint8_t interface_index;
} device_config_interface;

extern device_config_printer _printer_config;
device_config_printer       *printer_config;

extern uint8_t lpt_output(const uint8_t ch) __sdcccall(1);
extern void    drv_timi_printer(void);

usb_error find_printer(const uint8_t address, device_config_interface *const printer_interface) __sdcccall(1) {
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

      if (interface->bInterfaceClass == 7) {
        printer_interface->address         = address;
        printer_interface->config_index    = config_index;
        printer_interface->interface_index = interface_index;
        return USB_ERR_OK;
      }
    }
  }

  return 255;
}

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

#define H_LPTS_ADDR __at 0xFFBB
#define H_LPTO_ADDR __at 0xFFB6
#define RST_30      0xF7

#define POP_IX_1 0xDD
#define POP_IX_2 0xE1
#define OR       0xF6
#define RET      0xC9

typedef struct {
  uint8_t bytes[5];
} jump_hook_disable;

call_mapi_jump_hook_t H_LPTO_ADDR H_LPTO;
jump_hook_disable H_LPTS_ADDR     H_LPTS;

uint8_t install_printer(const uint16_t lpt_output) {
  if (install_segment_hook(lpt_output, &H_LPTO) == 0)
    return 255;

  H_LPTS.bytes[0] = POP_IX_1;
  H_LPTS.bytes[1] = POP_IX_2;
  H_LPTS.bytes[2] = OR;
  H_LPTS.bytes[3] = 0xFF;
  H_LPTS.bytes[4] = RET;

  return 0;
}

#define relocated(x)  ((void *)((uint16_t)(&x) + (uint16_t)(my_header)))
#define relocated2(x) ((void *)((uint16_t)(x) + (uint16_t)(my_header)))

uint16_t init(installed_sys_t *my_header) __z88dk_fastcall {
  usb_error result;

  printer_config = ((device_config_printer *)relocated(_printer_config));

  uint16_t      hl                  = extbio_usb_get_present();
  const uint8_t h                   = hl >> 8;
  const uint8_t last_device_address = hl & 0xFF;

  if (!h) {
    // printf("CH376 not detected\r\n");
    return 255;
  }

  device_config_interface printer_interface;
  memset(&printer_interface, 0, sizeof(device_config_interface));

  for (uint8_t address = 1; address <= last_device_address; address++) {
    result = find_printer(address, &printer_interface);

    if (result && result != 255) {
      printf("USB error %02x\r\n", result);
      return 255;
    }

    if (result == 0) {
      memset(printer_config, 0, sizeof(device_config_printer));

      result = construct_device_config(printer_interface.address, printer_interface.config_index, printer_interface.interface_index,
                                       (device_config *)printer_config);
      if (result) {
        printf("construct_device_config failed %d\r\n", result);
        return 255;
      }

      if (install_printer((uint16_t)relocated2(lpt_output))) {
        printf("Failed to install usb printer driver due to lack of jump hooks\r\n");
        return 255;
      }

      printf("PRNT.SYS:        INSTALLED\r\n");
      return 0;
    }
  }

  //no printer found -- return code to indicate service is not to be loaded.
  return 1;
}
