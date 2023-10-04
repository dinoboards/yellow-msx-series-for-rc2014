#include "main.h"
#include "enumerate_trace.h"
#include "print.h"
#include "printer_drv.h"
#include "usb-dev-info-ufi.h"
#include "usb-lun-info-ufi.h"
#include <ch376.h>
#include <class_printer.h>
#include <class_scsi.h>
#include <enumerate.h>

#include <class_ufi.h>
#include <delay.h>

#include "usb-dev-read-ufi.h"
#include "usb-dev-write-ufi.h"

uint16_t dumpSector(device_config *config, const uint32_t number) {
  usb_error error;

  uint8_t buffer[512];
  memset(buffer, 0, sizeof(buffer));
  uint8_t read = 0;

  printf("Sector %d\r\n", number);
  if ((error = usb_dev_read_ufi(config, 1, 1, number, buffer, &read)) != 0) {
    printf("usb_dev_read_ufi: %d, %d\n", error, read);
    return error;
  }

  for (int i = 0; i < 32; i++) {
    printf("%02X ", buffer[i]);
    if (i % 16 == 15)
      printf("\n");
  }
  return 0;
}

uint8_t writeSector(device_config *config, const uint32_t number) {
  usb_error error;

  uint8_t buffer[512];
  uint8_t written = 0;
  memset(buffer, 0, sizeof(buffer));
  buffer[3] = 0xF6;
  buffer[4] = number + 0x60;

  error = usb_dev_write_ufi(config, 1, 1, number, buffer, &written);
  printf("usb_dev_write_ufi: %d\n", error);
  return error;
}

void chput(const char c) __z88dk_fastcall { printf("%c", c); }

usb_error usb_host_bus_reset() {
  ch_cmd_set_usb_mode(CH_MODE_HOST);
  delay_short();

  ch_cmd_set_usb_mode(CH_MODE_HOST_RESET);
  delay_short();

  ch_cmd_set_usb_mode(CH_MODE_HOST);
  delay_short();

  ch_configure_nak_retry_3s();

  return USB_ERR_OK;
}

void right_trim(char *buffer) {
  for (int i = strlen(buffer) - 1; i >= 0; i--)
    if (buffer[i] == ' ')
      buffer[i] = 0;
    else
      break;
}

void state_devices(_usb_state *const work_area) __z88dk_fastcall {
  const bool hasUsb     = work_area->hub_config.address != 0;
  const bool hasCdc     = work_area->cdc_config.address != 0;
  const bool hasPrinter = work_area->printer_config.address != 0;

  uint8_t        index          = MAX_NUMBER_OF_STORAGE_DEVICES;
  device_config *storage_device = &work_area->storage_device[0];
  usb_error      result;

  uint8_t buffer[512];

  if (hasUsb)
    print_string("USB HUB:\r\n");
  else
    print_string("USB:\r\n");

  if (hasCdc)
    print_string("CDC\r\n");

  if (hasPrinter) {
    print_string("PRINTER\r\n");

    // result = prt_soft_reset(&work_area->printer);
    // printf(" prt_reset\r\n", result);

    // result = prt_get_port_status(&work_area->printer, buffer);
    // printf(" prt_get_port_status: %d, %02X\r\n", result, buffer[0]);

    // result = prt_get_device_id(&work_area->printer, buffer);
    // printf(" prt_get_device_id: %d, %02X%02X\r\n", result, buffer[0], buffer[1]);

    const char *str = "Verifying printer output works!\n";

    for (uint8_t p = 0; p < strlen(str); p++) {
      result = USBPRT(str[p]);

      if (result != USB_ERR_OK)
        printf(" USBPRT: %d, %d\r\n", p, result);
    }
  }

  do {
    const usb_device_type t = storage_device->type;
    memset(buffer, 0, sizeof(buffer));

    if (t == USB_IS_FLOPPY) {

      // ufi_request_sense_response sense;
      // memset(&sense, 0, sizeof(sense));

      // ufi_format_capacities_response r;
      // memset(&r, 0, sizeof(r));

      // ufi_inquiry_response const inquiry;
      // memset(&inquiry, 0, sizeof(inquiry));

      // // uint8_t counter = 5;

      // // result = wait_for_device_ready(storage_device, 5000);
      // // printf("wait_for_device_ready: %d\r\n", result);

      // // printf("--\r\n");

      // // result = ufi_read_format_capacities(storage_device, &r);
      // // printf("ufi_read_format_capacities: %d,", result);
      // // log_ufi_format_capacities_response(&r);
      // // delay(10);

      // // result = ufi_request_sense(storage_device, &sense);
      // // printf("ufi_request_sense: %d,", result);
      // // log_ufi_request_sense_response(&sense);

      // // result = ufi_inquiry(storage_device, &inquiry);
      // // printf("ufi_inquiry: %d,", result);
      // // log_usb_inquiry_response(&inquiry);
      // // printf("--\r\n");

      // // result = wait_for_device_ready(storage_device, 1000);
      // // printf("wait_for_device_ready: %d\r\n", result);

      // result = ufi_read_write_sector(storage_device, false, 1, 1, buffer);
      // printf("ufi_read_write_sector: %d\r\n", result);

      // result = ufi_request_sense(storage_device, &sense);
      // printf("ufi_request_sense: %d,", result);
      // log_ufi_request_sense_response(&sense);

      writeSector(storage_device, 6);
      // writeSector(storage_device, 7);
      // writeSector(storage_device, 8);

      dumpSector(storage_device, 6);

      // dumpSector(storage_device, 7);
      // dumpSector(storage_device, 8);

      // const usb_error result = ufi_request_sense(dev, &response);

      // nextor_lun_info info;
      // memset(&info, 0, sizeof(info));

      // print_string("UFI\r\n   MANUFACTURER NAME: ");

      // usb_dev_info_ufi((device_config *const)storage_device, MANUFACTURER_NAME, (uint8_t *const)buffer);

      // right_trim(buffer);
      // print_string(buffer);
      // print_string("\r\n   SECTOR_SIZE: ");

      // usb_lun_info_ufi((device_config *const)storage_device, 1, &info);

      // printf("%d\r\n   NUMBER_OF_SECTORS: %ld\r\n", info.sector_size, info.number_of_sectors);
    } else if (t == USB_IS_MASS_STORAGE) {
      print_string("SCSI\r\n  VENDOR NAME: ");

      scsi_inquiry_result response;
      memset(&response, 0, sizeof(scsi_inquiry_result));
      const usb_error result = scsi_inquiry((device_config *const)storage_device, &response);

      memcpy(buffer, response.vendor_information, 8);
      right_trim(buffer);
      print_string(buffer);
      print_string("\r\n");

      scsi_read_capacity_result scsi_response;
      get_scsi_read_capacity((device_config *const)storage_device, &scsi_response);

      uint16_t sector_size = scsi_response.block_size[2] << 8 + scsi_response.block_size[3];
      uint32_t number_of_sectors;

      uint8_t *      no_sectors = ((uint8_t *)&number_of_sectors);
      const uint8_t *no_blocks  = scsi_response.number_of_blocks + 3;

      *no_sectors++ = *no_blocks--;
      *no_sectors++ = *no_blocks--;
      *no_sectors++ = *no_blocks--;
      *no_sectors   = *no_blocks--;

      printf("  SECTOR_SIZE: %d\r\n  NUMBER_OF_SECTORS: %ld\r\n", sector_size, number_of_sectors);
    }

    storage_device++;
  } while (--index != 0);
}

inline void initialise_scsi_devices(_usb_state *const work_area) {
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

  _usb_state *const work_area = get_usb_work_area();

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

      initialise_scsi_devices(work_area);

      state_devices(work_area);

      return;
    }
  }

  printf("DISCONNECTED\r\n");
}
