#include "arguments.h"
#include "command_floppy_find.h"
#include "command_floppy_report.h"
#include "command_report_all_devices.h"
#include "device_search.h"
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

  case cmd_floppy_find:
    return floppy_find();
  }

  return 0;
}
