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

inline uint8_t usb_host_bus_reset() {
  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60 / 4);

  ch376_set_usb_mode(CH_MODE_HOST_RESET);
  delay(60 / 2);

  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60 / 4);

  hw_configure_nak_retry();

  return true;
}

uint8_t read_all_configs() {
  device_descriptor        desc;
  uint8_t                  result;
  uint16_t                 amount_transferred = 0;
  uint8_t                  buffer[140];
  config_descriptor *const config_desc = (config_descriptor *)buffer;

  result = hw_get_description(0, &desc);
  printf("Desc %02x\r\n", result);
  logDevice(&desc);

  for (uint8_t config_index = 0; config_index < desc.bNumConfigurations; config_index++) {
    printf("Config %d: ", config_index);

    result = hw_get_config_descriptor(config_desc, config_index, desc.bMaxPacketSize0, sizeof(config_descriptor), 0, &amount_transferred);
    if (result != CH_USB_INT_SUCCESS) {
      yprintf(15, "X1 (%d)", result);
      return result;
    }
    logConfig(config_desc);

    result = hw_get_config_descriptor(config_desc, config_index, desc.bMaxPacketSize0, config_desc->wTotalLength, 0, &amount_transferred);
    if (result != CH_USB_INT_SUCCESS) {
      yprintf(15, "X2 (%d)", result);
      return result;
    }

    uint8_t floppyInterface = 255;
    uint8_t massStorageInterface = 255;

    const interface_descriptor *p = (const interface_descriptor *)(buffer + sizeof(config_descriptor));
    for (uint8_t interface_index = 0; interface_index < config_desc->bNumInterfaces; interface_index++) {
      printf("Interf %d: ", interface_index);
      logInterface(p);

      if (p->bInterfaceClass == 8 && p->bInterfaceSubClass == 6 && p->bInterfaceProtocol == 80 && massStorageInterface == 255)
        massStorageInterface = p->bInterfaceNumber;

      if (p->bInterfaceClass == 8 && p->bInterfaceSubClass == 4 && p->bInterfaceProtocol == 0 && floppyInterface == 255)
        floppyInterface = p->bInterfaceNumber;

      const endpoint_descriptor *pEndpoint = (const endpoint_descriptor *)(p + 1);

      for (uint8_t endpoint_index = 0; endpoint_index < p->bNumEndpoints; endpoint_index++) {
        printf("end %p: ", endpoint_index);

        logEndPointDescription(pEndpoint);
        pEndpoint++;
      }

      p = (interface_descriptor *)pEndpoint;
    }

    if (floppyInterface != 255) {
      printf("Detected floppy at %d", floppyInterface);
    }

    if (massStorageInterface != 255) {
      printf("Detected mass storage at %d", massStorageInterface);
    }
  }

  return 0;
}

uint8_t usb_host_init() {
  // work_area *const p = get_work_area();
  // printf("usb_host_init %p\r\n", p);

  __asm__("EI");

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

  read_all_configs();

  return 0;
}
