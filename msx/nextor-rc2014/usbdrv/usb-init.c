#include "nextor.h"
#include "print.h"
#include "printer_drv.h"
#include "usb-dev.h"
#include "usb-lun-info.h"
#include "work-area.h"
#include <ch376.h>
#include <class_scsi.h>
#include <delay.h>
#include <enumerate.h>
#include <enumerate_hub.h>
#include <enumerate_trace.h>
#include <protocol.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

usb_error usb_host_bus_reset() {
  ch_cmd_set_usb_mode(CH_MODE_HOST);
  delay_medium();

  ch_cmd_set_usb_mode(CH_MODE_HOST_RESET);
  delay_medium();

  ch_cmd_set_usb_mode(CH_MODE_HOST);
  delay_medium();

  ch_configure_nak_retry_3s();

  return USB_ERR_OK;
}

void convert_and_print_disk_size(uint32_t number_of_sectors) {
  if (number_of_sectors == 1474560 / 512) {
    print_string("1440KB");
    return;
  }

  const char *suffix = "KB";

  if (number_of_sectors >= 2048) {
    number_of_sectors /= 256;
    suffix = "MB";
  }

  if (number_of_sectors >= 2048) {
    number_of_sectors /= 1024;
    suffix = "GB";
  }

  const uint16_t a = (uint16_t)(number_of_sectors / 8);
  const uint16_t b = (uint16_t)(number_of_sectors % 8 * 100) / 8;

  print_uint16(a);

  if (b != 0) {
    print_string(".");
    print_uint16(b);
  }
  print_string(suffix);
}

void print_disk_size(const uint8_t device_index) {
  nextor_lun_info info;

  usb_lun_info(device_index, 1, &info);
  convert_and_print_disk_size(info.number_of_sectors);
  print_string(")\r\n");
}

bool state_devices(const _usb_state *const work_area) __z88dk_fastcall {
  const bool hasUsbHub  = work_area->hub_config.address != 0;
  const bool hasCdc     = work_area->cdc_config.address != 0;
  const bool hasPrinter = work_area->printer_config.address != 0;

  uint8_t storage_count = 0;
  uint8_t index         = 1; // MAX_NUMBER_OF_STORAGE_DEVICES;
  // const device_config *storage_device = &work_area->storage_device[0];

  if (hasUsbHub)
    print_string("USB HUB:\r\n");
  else
    print_string("USB:         ");

  if (hasCdc)
    print_string("    CDC\r\n");

  if (hasPrinter)
    print_string("    PRINTER\r\n");

  do {
    const device_config *const storage_device = get_usb_driver(index);
    const usb_device_type      t              = storage_device->type;
    if (t == USB_IS_FLOPPY) {
      print_string("    FLOPPY  (");
      print_disk_size(index);
      storage_count++;

    } else if (t == USB_IS_MASS_STORAGE) {
      print_string("    STORAGE (");
      print_disk_size(index);
      storage_count++;
    }

  } while (++index != MAX_NUMBER_OF_STORAGE_DEVICES);

  if (!hasUsbHub && !hasCdc && !hasPrinter && storage_count == 0) {
    print_string("\r\n");
    return false;
  }

  return storage_count != 0;
}

inline void initialise_mass_storage_devices(_usb_state *const work_area) {
  uint8_t        index          = MAX_NUMBER_OF_STORAGE_DEVICES;
  device_config *storage_device = &work_area->storage_device[0];

  do {
    if (storage_device->type == USB_IS_MASS_STORAGE)
      scsi_sense_init(storage_device);
    storage_device++;
  } while (--index != 0);
}

#define ERASE_LINE "\x1B\x6C\r"

uint8_t usb_host_init(const uint8_t flag) __z88dk_fastcall {
  if (flag == 0) {
    ch_cmd_reset_all();

    return 0;
  }

  work_area *const p = get_work_area();
  __asm__("EI");
  _usb_state *const work_area = &p->ch376;
  memset(work_area, 0, sizeof(_usb_state));

  delay_short();

  if (!ch_probe()) {
    print_string("CH376:           NOT PRESENT\r\n");
    return false;
  }

  const uint8_t ver = ch_cmd_get_ic_version();
  print_string("CH376:           PRESENT (VER ");
  print_hex(ver);
  print_string(")\r\n");

  print_string("USB:             SCANNING...");

  usb_host_bus_reset();

  for (uint8_t i = 0; i < 4; i++) {
    const uint8_t r = ch_very_short_wait_int_and_get_status();

    if (r == USB_INT_CONNECT) {
      enumerate_all_devices();

      initialise_mass_storage_devices(work_area);

      install_printer(work_area);

      print_string(ERASE_LINE);

      return state_devices(work_area);
    }
  }

  print_string(ERASE_LINE);

  print_string("USB:             DISCONNECTED\r\n");

  return false;
}
