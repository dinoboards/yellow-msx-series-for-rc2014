#include "ch376.h"
#include "hw.h"
#include "nextor.h"
#include "scsi.h"
#include "usb-enumerate-hub.h"
#include "usb-enumerate.h"
#include "usb.h"
#include "work-area.h"
#include <delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "debuggin.h"
#include "print.h"

usb_error usb_host_bus_reset() {
  ch376_set_usb_mode(CH_MODE_HOST);

  ch376_set_usb_mode(CH_MODE_HOST_RESET);

  ch376_set_usb_mode(CH_MODE_HOST);

  ch_configure_nak_retry_indefinite();

  return USB_ERR_OK;
}

bool state_devices(const _usb_state *const work_area) {
  const bool hasUsb = work_area->hub_config.address != 0;

  uint8_t floppy_count       = 0;
  uint8_t mass_storage_count = 0;

  for (int8_t index = MAX_NUMBER_OF_STORAGE_DEVICES - 1; index >= 0; index--) {
    const usb_device_type t = work_area->storage_device[index].type;
    if (t == USB_IS_FLOPPY)
      floppy_count++;
    else if (t == USB_IS_MASS_STORAGE)
      mass_storage_count++;
  }

  if (!hasUsb && floppy_count == 0 && mass_storage_count == 0)
    return false;

  print_string("USB:             ");

  if (hasUsb) {
    print_string("HUB");

    if (floppy_count != 0 || mass_storage_count != 0)
      print_string(", ");
  }

  if (floppy_count != 0) {
    print_string("FLOPPY");

    if (mass_storage_count != 0)
      print_string(", ");
  }

  if (mass_storage_count != 0)
    print_string("STORAGE");

  print_string("\r\n");

  return floppy_count != 0 || mass_storage_count != 0;
}

uint8_t usb_host_init() {
  work_area *const p = get_work_area();
  __asm__("EI");
  _usb_state *const work_area = &p->ch376;
  memset(work_area, 0, sizeof(_usb_state));

  ch376_reset();

  if (!ch376_probe()) {
    print_string("CH376:           NOT PRESENT\r\n");
    return false;
  }

  const uint8_t ver = ch376_get_firmware_version();
  print_string("CH376:           PRESENT (VER ");
  print_hex(ver);
  print_string(")\r\n");

  usb_host_bus_reset();
  delay(10);

  enumerate_all_devices();

  const usb_error result = state_devices(work_area);

  uint8_t                mass_storage_count = 0;
  storage_device_config *storage_device     = &work_area->storage_device[0];

  for (int8_t index = MAX_NUMBER_OF_STORAGE_DEVICES - 1; index >= 0; index--) {
    if (storage_device->type == USB_IS_MASS_STORAGE)
      scsi_sense_init(storage_device);
    storage_device++;
  }

  // printf("Doing a read\r\n");
  // uint8_t buffer[512];
  // const usb_error read_result = scsi_read(&work_area->storage_device[0], 0, buffer);
  // printf("result %d, %02x %02x %02x %02x", read_result, buffer[0], buffer[1], buffer[2], buffer[3]);

  return result;
}
