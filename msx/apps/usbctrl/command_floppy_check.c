#include "command_floppy_check.h"
#include "device_search.h"
#include <class_ufi.h>
#include <msxdos.h>
#include <string.h>

uint8_t command_floppy_check(void) __sdcccall(1) {
  usb_error result;
  uint8_t   buffer[512 * 18];

  device_config storage_device;
  memset(&storage_device, 0, sizeof(storage_device));

  result = get_device_config_from_drive_letter(&storage_device);
  if (result) {
    printf("Not a floppy or mapped drive\r\n");
    return result;
  }

  ufi_format_capacities_response r;
  memset(&r, 0, sizeof(r));

  result = retrieve_floppy_formats(&storage_device, &r);
  if (result) {
    printf("retrieve_floppy_formats failed with error %d\r\n", result);
    return result;
  }

  const uint16_t number_of_blocks = convert_from_msb_first(r.descriptors[0].number_of_blocks);
  for (int16_t sector_number = 0; sector_number < number_of_blocks; sector_number += 9) {
    if (msxbiosBreakX()) {
      printf("\r\n Aborted\r\n");
      return 1;
    }

    printf("\rChecking Sector %4d to %4d out of %d   ", sector_number, sector_number + 9, number_of_blocks);

    if (wait_for_device_ready(&storage_device, 1000) != 0) {
      printf("not ready\r\n");
      return 255;
    }

    result = ufi_read_write_sector(&storage_device, false, sector_number, 9, buffer, NULL);
    if (result) {
      printf("read for sector %d failed with error %d\r\n", sector_number, result);
    }
  }

  printf("\r\n");

  return 0;
}
