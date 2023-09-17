#include "main.h"
#include "ch376.h"
#include "class-printer.h"
#include "class-scsi.h"
#include "debuggin.h"
#include "print.h"
#include "usb-dev-info-ufi.h"
#include "usb-enumerate.h"
#include "usb-lun-info-ufi.h"

void chput(const char c) __z88dk_fastcall { printf("%c", c); }

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

void state_devices(_usb_state *const work_area) __z88dk_fastcall {
  const bool hasUsb     = work_area->hub_config.address != 0;
  const bool hasCdc     = work_area->cdc_config.address != 0;
  const bool hasPrinter = work_area->printer.address != 0;

  uint8_t              index          = MAX_NUMBER_OF_STORAGE_DEVICES;
  const device_config *storage_device = &work_area->storage_device[0];

  uint8_t buffer[130];
  memset(buffer, 0, sizeof(buffer));

  if (hasUsb)
    print_string("USB HUB:\r\n");
  else
    print_string("USB:\r\n");

  if (hasCdc)
    print_string("CDC\r\n");

  if (hasPrinter) {
    print_string("PRINTER: ");

    logEnabled = true;

    // uint8_t status;
    usb_error result;

    result = prt_send_text(&work_area->printer, "Hello World\r\n");

    printf(" send: %d\r\n", result);

    // usb_error result = prt_soft_reset(&work_area->printer_config);

    // printf(" reset: %d\r\n", result);

    // result = prt_get_device_id(&work_area->printer_config, buffer);

    // printf(" id: %d, %02X %02X %c\r\n", result, buffer[0], buffer[1], buffer[2]);

    // result = prt_get_port_status(&work_area->printer_config, &status);

    // printf(" result: %02X, status: %02X\r\n", result, status);
  }

  nextor_lun_info info;
  memset(&info, 0, sizeof(info));

  do {
    const usb_device_type t = storage_device->type;
    if (t == USB_IS_FLOPPY) {
      print_string("FLOPPY\r\nMANUFACTURER_NAME: ");

      usb_dev_info_ufi((device_config *const)storage_device, MANUFACTURER_NAME, (uint8_t *const)buffer);
      print_string(buffer);
      print_string("SECTOR_SIZE: ");

      usb_lun_info_ufi((device_config *const)storage_device, 1, &info);

      printf("%d\r\nNUMBER_OF_SECTORS: %ld\r\n", info.sector_size, info.number_of_sectors);
    } else if (t == USB_IS_MASS_STORAGE) {
      print_string("STORAGE\r\n");

    } else if (t == USB_IS_HUB) {
      print_string("HUB\r\n");
    }

    storage_device++;
  } while (--index != 0);
}

inline void initialise_mass_storage_devices(_usb_state *const work_area) {
  uint8_t        index          = MAX_NUMBER_OF_STORAGE_DEVICES;
  device_config *storage_device = &work_area->storage_device[0];

  do {
    if (storage_device->type == USB_IS_MASS_STORAGE) {
      const usb_error result = scsi_sense_init(storage_device);
    }

    storage_device++;
  } while (--index != 0);
}

#define ERASE_LINE "\x1B\x6C\r"

void main(const int argc, const char *argv[]) {
  (void)argc;
  (void)argv;

  work_area *const  p         = get_work_area();
  _usb_state *const work_area = &p->ch376;
  memset(work_area, 0, sizeof(_usb_state));

  ch_cmd_reset_all();

  if (!ch_probe()) {
    printf("CH376:           NOT PRESENT\r\n");
    return;
  }

  const uint8_t ver = ch_cmd_get_ic_version();
  printf("CH376:           PRESENT (VER ");
  printf("%d", ver);
  printf(")\r\n");

  printf("USB:             SCANNING...\r\n");

  usb_host_bus_reset();

  for (uint8_t i = 0; i < 4; i++) {
    const uint8_t r = ch_very_short_wait_int_and_get_status();

    if (r == USB_INT_CONNECT) {
      enumerate_all_devices();

      initialise_mass_storage_devices(work_area);

      state_devices(work_area);

      return;
    }
  }

  printf("DISCONNECTED\r\n");
}
