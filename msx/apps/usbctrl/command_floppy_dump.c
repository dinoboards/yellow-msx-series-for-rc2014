#include "command_floppy_check.h"
#include "device_search.h"
#include "read_and_write_sectors.h"
#include <class_ufi.h>
#include <msxdos.h>
#include <string.h>

uint8_t command_floppy_dump(void) __sdcccall(1) {
  usb_error result;
  uint8_t   buffer[512];

  device_config storage_device;
  memset(&storage_device, 0, sizeof(storage_device));

  result = get_device_config_from_drive_letter(&storage_device);
  if (result) {
    printf("Not a floppy or mapped drive\r\n");
    return result;
  }

  result = read_sector(&storage_device, 0, 1, buffer);
  if (result) {
    printf("read_sector failed with error %d\r\n", result);
    return result;
  }

  printf("\r\n");

  for (int i = 0; i < 128; i++) {
    printf("%02X ", buffer[i]);
    if (i % 16 == 15)
      printf("\n");
  }

  return 0;
}
