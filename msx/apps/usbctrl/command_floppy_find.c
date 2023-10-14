#include "command_floppy_find.h"
#include "arguments.h"
#include "device_search.h"
#include "format_mapping.h"
#include "ufi_reporters.h"
#include <msxdos.h>
#include <msxdos_rc2014_ex.h>
#include <stdio.h>

usb_error command_floppy_find(void) {
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

  log_ufi_format_capacities_response(&r);

  const disk_geometry *const geometry = size_to_geometry(convert_from_msb_first(&r.descriptors[0].number_of_blocks[0]));

  if (geometry) {
    printf("Total Sectors: %d\r\n", geometry->number_of_blocks);
    printf("Tracks: %d\r\n", geometry->tracks);
    printf("Sides: %d\r\n", geometry->sides);
    printf("Sectors per track: %d\r\n", geometry->sectors_per_track);
    printf("Description: %s\r\n", geometry->description);
  }

  return result;
}
