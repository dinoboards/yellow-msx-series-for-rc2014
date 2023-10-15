#include "command_floppy_format.h"
#include "device_search.h"
#include "format_fats.h"
#include "format_mapping.h"
#include "usb_trace.h"
#include <class_ufi.h>
#include <string.h>

usb_error format_tracks(device_config *const storage_device, const ufi_format_capacity_descriptor *const descriptor) __sdcccall(1) {
  usb_error result;

  for (uint8_t track_number = 0; track_number < 80; track_number++)
    for (uint8_t side = 0; side < 2; side++) {
      wait_for_device_ready(storage_device, 2000);

      printf("formating track: %d, side: %d\r", track_number, side);
      result = ufi_format(storage_device, side, track_number, descriptor);

      if (result) {
        printf("\rFormat failure @ %d, %d: %d\r\n", track_number, side, result);
        return result;
      }

      ufi_request_sense_response response;
      memset(&response, 0, sizeof(response));

      if ((result = ufi_request_sense(storage_device, &response)) != USB_ERR_OK)
        return result;

      const uint8_t asc       = response.asc;
      const uint8_t ascq      = response.ascq;
      const uint8_t sense_key = response.sense_key;

      if (sense_key != 0) {
        printf("\rFormat failure @ %d, %d, %d\r\n", sense_key, asc, ascq);
        return 255;
      }
    }

  return 0;
}

usb_error command_floppy_format(void) __sdcccall(1) {
  usb_error result;

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
  if (result)
    return result;

  const disk_geometry *const geometry = size_to_geometry(convert_from_msb_first(&r.descriptors[0].number_of_blocks[0]));

  if (geometry == NULL) {
    printf("Unsupported disk geometry/size\r\n");
    return 255;
  }

  printf("Total Sectors: %d\r\n", geometry->number_of_blocks);
  printf("Tracks:        %d\r\n", geometry->tracks);
  printf("Sides:         %d\r\n", geometry->sides);
  printf("Sectors/track: %d\r\n", geometry->sectors_per_track);
  printf("Description:   %s\r\n", geometry->description);

  printf("WARNING, ALL DATA ON DRIVE %c: WILL BE LOST!\r\nProceed with Format (Y/N)? ", floppy_drive_letter);

  char c = getchar();
  if (c != 'Y' && c != 'y') {
    printf("\r\n");
    return 255;
  }

  printf("\r\n");

  if (!quick_format) {
    result = format_tracks(&storage_device, &r.descriptors[0]);
    if (result)
      return result;
  }

  result = format_fat(&storage_device, geometry);

  printf("\r\nFormat completed.\r\n");

  return 0;
}
