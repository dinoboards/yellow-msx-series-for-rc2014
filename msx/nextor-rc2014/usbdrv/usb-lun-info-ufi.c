#include "usb-lun-info-ufi.h"
#include <class_ufi.h>
#include <format_mapping.h>
#include <stdlib.h>
#include <string.h>

uint8_t usb_lun_info_ufi(device_config *const dev, const uint8_t lun, nextor_lun_info *const info) {
  ufi_format_capacities_response response;
  memset(&response, 0, sizeof(ufi_format_capacities_response));
  memset(info, 0, sizeof(nextor_lun_info));

  if (lun != 1)
    return 1;

  wait_for_device_ready(dev, 2500);

  // not sure if we need to do this to 'clear' some state
  ufi_inquiry_response inquiry;
  ufi_inquiry(dev, &inquiry);

  wait_for_device_ready(dev, 1500);

  const usb_error result = ufi_read_format_capacities(dev, &response);
  if (result != USB_ERR_OK)
    return 1;

  info->sector_size       = response.descriptors[0].block_size[1] << 8 + response.descriptors[0].block_size[0];
  info->number_of_sectors = convert_from_msb_first(response.descriptors[0].number_of_blocks);

  const disk_geometry *const geometry = size_to_geometry(info->number_of_sectors);

  info->flags                       = INFO_FLAG_REMOVABLE | INFO_FLAG_FLOPPY;
  info->number_of_cylinders         = geometry->tracks;
  info->number_of_heads             = geometry->sides;
  info->number_of_sectors_per_track = geometry->sectors_per_track;

  return 0;
}
