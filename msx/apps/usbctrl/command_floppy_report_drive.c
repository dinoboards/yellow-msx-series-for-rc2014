#include "command_floppy_report_drive.h"
#include "arguments.h"
#include "device_search.h"
#include "ufi_reporters.h"
#include <format_mapping.h>
#include <msxdos.h>
#include <msxdos_rc2014_ex.h>
#include <stdio.h>

usb_error command_floppy_report_drive(void) {
  driveLetterInfo info;
  memset(&info, 0, sizeof(driveLetterInfo));

  uint8_t result = msxdosGetDriveLetterInfo(floppy_drive_letter - 'A', &info);
  if (result) {
    printf("msxdosGetDriveLetterInfo failed %d\r\n", result);
    return result;
  }

  rc2014LunInfoExtended lunInfoEx;
  memset(&lunInfoEx, 0, sizeof(rc2014LunInfoExtended));

  uint16_t x = rc2014GetLunInfoEx(info.phyical.slot, info.phyical.index, info.phyical.lun, &lunInfoEx);
  if (!(lunInfoEx.extendedFlags & 0x01)) {
    printf("Not USB floppy\r\n");
    return 255;
  }

  ufi_format_capacities_response r;
  memset(&r, 0, sizeof(r));

  device_config storage_device;
  memset(&storage_device, 0, sizeof(storage_device));

  result = get_device_config_from_drive_letter(&storage_device);
  if (result)
    return result;

  result = retrieve_floppy_formats(&storage_device, &r);
  if (result)
    return result;

  switch (r.descriptors[0].descriptor_code) {
  case UFI_DESCRIPTOR_CODE_UNFORMATTED_MEDIA:
    printf("Unformatted media\r\n");
    break;
  case UFI_DESCRIPTOR_CODE_FORMATTED_MEDIA:
    printf("Formatted media\r\n");
    break;
  case UFI_DESCRIPTOR_CODE_NO_MEDIA:
    printf("No media\r\nDefault Format:\r\n");
    break;
  default:
    printf("Unknown media\r\n");
    break;
  }

  const uint32_t number_of_blocks = convert_from_msb_first(&r.descriptors[0].number_of_blocks[0]);
  const uint32_t sector_size      = (uint32_t)r.descriptors[0].block_size[0] << 16 | (uint16_t)r.descriptors[0].block_size[1] << 8 |
                               r.descriptors[0].block_size[2];

  const disk_geometry *const geometry = size_to_geometry(number_of_blocks);

  if (geometry) {
    printf("  Total Sectors: %d\r\n", (uint16_t)number_of_blocks);
    printf("  Sector Size: %ld\r\n", sector_size);
    printf("  Tracks: %d\r\n", geometry->tracks);
    printf("  Sides: %d\r\n", geometry->sides);
    printf("  Sectors per track: %d\r\n", geometry->sectors_per_track);
    printf("  Description: %s\r\n", geometry->description);
  } else {
    printf(" Unknown geometry\r\n");
    printf(" Total Sectors: %ld\r\n", (uint32_t)number_of_blocks);
  }

  return result;
}
