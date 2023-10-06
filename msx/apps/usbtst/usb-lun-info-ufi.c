#include "usb-lun-info-ufi.h"
#include "class_ufi.h"
#include <stdlib.h>

uint8_t usb_lun_info_ufi(device_config *const dev, const uint8_t lun, nextor_lun_info *const info) {
  ufi_format_capacities_response response;

  if (lun != 1)
    return 1;

  const usb_error result = ufi_read_format_capacities(dev, &response);
  if (result != USB_ERR_OK)
    return 1;

  info->medium_type = 0; // block_device
  info->sector_size = response.block_size[1] << 8 + response.block_size[0];

  uint8_t       *no_sectors = ((uint8_t *)&info->number_of_sectors);
  const uint8_t *no_blocks  = response.number_of_blocks + 3;

  *no_sectors++ = *no_blocks--;
  *no_sectors++ = *no_blocks--;
  *no_sectors++ = *no_blocks--;
  *no_sectors   = *no_blocks--;

  info->flags                       = INFO_FLAG_REMOVABLE | INFO_FLAG_FLOPPY;
  info->number_of_cylinders         = 0;
  info->number_of_heads             = 0;
  info->number_of_sectors_per_track = 0;

  return 0;
}
