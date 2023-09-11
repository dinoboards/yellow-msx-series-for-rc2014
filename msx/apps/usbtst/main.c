#include "main.h"
#include "ch376.h"
#include "print.h"
#include "scsi.h"
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

void state_devices(const _usb_state *const work_area) __z88dk_fastcall {
  const bool hasUsb = work_area->hub_config.address != 0;

  uint8_t floppy_count = 0;
  uint8_t mass_storage_count = 0;
  // uint8_t                      ethernet_adapter = 0;
  uint8_t                      index = MAX_NUMBER_OF_STORAGE_DEVICES;
  const storage_device_config *storage_device = &work_area->storage_device[0];

  print_string("USB:\r\n");

  uint8_t buffer[130];
  memset(buffer, 0, sizeof(buffer));

  nextor_lun_info info;
  memset(&info, 0, sizeof(info));

  do {
    const usb_device_type t = storage_device->type;
    if (t == USB_IS_FLOPPY) {
      print_string("FLOPPY\r\nMANUFACTURER_NAME: ");

      usb_dev_info_ufi((storage_device_config *const)storage_device, MANUFACTURER_NAME, (uint8_t *const)buffer);
      print_string(buffer);
      print_string("SECTOR_SIZE: ");

      usb_lun_info_ufi((storage_device_config *const)storage_device, 1, &info);

      printf("%d\r\nNUMBER_OF_SECTORS: %ld\r\n", info.sector_size, info.number_of_sectors);
    } else if (t == USB_IS_MASS_STORAGE) {
      print_string("STORAGE\r\n");

    } else if (t == USB_IS_HUB) {
      print_string("HUB\r\n");
    }

    // else if (t == USB_IS_MASS_STORAGE_OF_ETHERNET_ADAPTER)
    //   ethernet_adapter++;

    storage_device++;
  } while (--index != 0);
}

inline void initialise_mass_storage_devices(_usb_state *const work_area) {
  uint8_t                index = MAX_NUMBER_OF_STORAGE_DEVICES;
  storage_device_config *storage_device = &work_area->storage_device[0];

  do {
    if (storage_device->type == USB_IS_MASS_STORAGE /*|| storage_device->type == USB_IS_MASS_STORAGE_OF_ETHERNET_ADAPTER*/) {
      scsi_sense_init(storage_device);

      // if (storage_device->type == USB_IS_MASS_STORAGE_OF_ETHERNET_ADAPTER)
      //   scsi_eject(storage_device);
    }

    storage_device++;
  } while (--index != 0);
}

#define ERASE_LINE "\x1B\x6C\r"

void main(const int argc, const char *argv[]) {
  (void)argc;
  (void)argv;

  work_area *const  p = get_work_area();
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

  for (int i = 0; i < 4; i++) {
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
