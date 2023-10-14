#include "command_floppy_format.h"
#include "device_search.h"
#include "usb_trace.h"
#include <class_ufi.h>
#include <string.h>

usb_error command_floppy_format(void) __sdcccall(1) {
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
