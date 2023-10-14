#include "command_floppy_check.h"
#include "device_search.h"
#include <class_ufi.h>
#include <string.h>

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
