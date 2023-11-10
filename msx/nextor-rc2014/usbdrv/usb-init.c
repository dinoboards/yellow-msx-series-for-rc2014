#include "nextor.h"
#include "print.h"
// #include "printer_drv.h"
#include "usb-dev.h"
#include "usb-lun-info.h"
#include "work-area.h"
#include <ch376.h>
#include <class_scsi.h>
#include <delay.h>
#include <enumerate.h>
#include <enumerate_hub.h>
#include <protocol.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

usb_error usb_host_bus_reset(void) {
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
    print_string("1.44MB");
    return;
  }

  const char *suffix = "KB";

  if (number_of_sectors <= 2880) {
    print_uint16(number_of_sectors / 2);
    print_string(suffix);
    return;
  }

  number_of_sectors /= 256;
  suffix = "MB";

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
  memset(&info, 0, sizeof(info));

  usb_lun_info(device_index, 1, &info);
  convert_and_print_disk_size(info.number_of_sectors);
  print_string(")\r\n");
}

bool state_devices(const _usb_state *const boot_area) __z88dk_fastcall {

  uint8_t storage_count = 0;
  uint8_t index         = 1; // MAX_NUMBER_OF_STORAGE_DEVICES;

  const uint16_t size = boot_area->bytes_required;
  print_string("USB:");
  if (size > 0) {
    print_string("             (");
    print_uint16(size);
    print_string(" bytes)");
  }
  print_string("\r\n");

  const bool hasCdc = find_device_config(USB_IS_CDC) != NULL;
  if (hasCdc)
    print_string("    CDC\r\n");

  const bool hasUnknown = find_device_config(USB_IS_UNKNOWN) != NULL;
  if (hasUnknown)
    print_string("    UNKNOWN <DRIVER REQUIRED>\r\n");

  const bool hasKeyboard = find_device_config(USB_IS_KEYBOARD) != NULL;
  if (hasKeyboard)
    print_string("    KEYBOARD\r\n");

  do {
    const device_config *const storage_device = get_usb_device_config(index);
    if (storage_device == NULL)
      break;

    const usb_device_type t = storage_device->type;
    if (t == USB_IS_FLOPPY) {
      print_string("    FLOPPY  (");
      print_disk_size(index);
      storage_count++;

    } else if (t == USB_IS_MASS_STORAGE) {
      print_string("    STORAGE (");
      print_disk_size(index);
      storage_count++;
    }

  } while (++index != MAX_NUMBER_OF_STORAGE_DEVICES + 1);

  if (!hasCdc && !hasUnknown && storage_count == 0) {
    print_string("    DISCONNECTED\r\n");
    return false;
  }

  return storage_count != 0;
}

void initialise_scsi_devices(void) {
  const _usb_state *const boot_state = get_usb_boot_area();

  for (device_config *storage_device = first_device_config(boot_state); storage_device;
       storage_device                = next_device_config(boot_state, storage_device))
    if (storage_device->type == USB_IS_MASS_STORAGE)
      scsi_sense_init(storage_device);
}

#define ERASE_LINE "\x1B\x6C\r"

/**
 * @brief enumerate all usb devices, noting them in usb_boot_area (0xC000)
 *
 * @return uint16_t the number of bytes required to hold usb configuration
 */
uint16_t boot_phase_1(void) {
  _usb_state *const boot_state = get_usb_boot_area();
  memset(boot_state, 0, sizeof(_usb_state));

  ch_cmd_reset_all();

  if (!ch_probe()) {
    print_string("CH376:           NOT PRESENT\r\n");
    goto finally;
  }

  boot_state->connected = true;

  print_string("CH376:           PRESENT (VER ");
  print_hex(ch_cmd_get_ic_version());
  print_string(")\r\n");

  print_string("USB:             SCANNING...");

  usb_host_bus_reset();

  for (uint8_t i = 0; i < 4; i++) {
    const uint8_t r = ch_very_short_wait_int_and_get_status();

    if (r == USB_INT_CONNECT) {
      enumerate_all_devices();

      initialise_scsi_devices();

      goto finally;
    }
  }

finally:
  print_string(ERASE_LINE);

  const uint16_t last = (uint16_t)find_first_free();

  boot_state->bytes_required = (last - (uint16_t)boot_state) + 1;

  return boot_state->bytes_required;
}

/**
 * @brief state the discovered usb devices and copy from
 * temp boot usb store to permenant usb work area
 *
 * @return uint16_t the number of usb drives found
 *                  (floppy and or mass storage)
 */
uint16_t boot_phase_2(void) {
  _usb_state *const boot_state = get_usb_boot_area();
  work_area *const  p          = get_work_area();
  __asm__("EI");
  _usb_state *const usb_state = &p->ch376;
  memcpy(usb_state, boot_state, boot_state->bytes_required);

  if (!boot_state->connected) {
    return 0;
  }

  p->present |= PRES_CH376;

  // install_printer();

  state_devices(boot_state);

  return get_number_of_usb_drives();
}

uint16_t usb_host_init(const uint8_t flag) __z88dk_fastcall {
  if (flag == 0)
    return boot_phase_1();

  return boot_phase_2();
}
