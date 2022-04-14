#include "scsi.h"
#include "usb-lun-info-ufi.h"
#include <stdlib.h>

#include "print.h"

uint8_t usb_lun_info_scsi(storage_device_config *const dev, const uint8_t lun, nextor_lun_info *const info) {
  scsi_read_capacity_result response;

  if (lun != 1)
    return 1;

  const usb_error result = get_scsi_read_capacity(dev, &response);
  if (result != USB_ERR_OK)
    return 1;

  info->medium_type = 0; // block_device
  info->sector_size = response.block_size[2] << 8 + response.block_size[3];

  uint8_t *      no_sectors = ((uint8_t *)&info->number_of_sectors);
  const uint8_t *no_blocks  = response.number_of_blocks + 3;

  *no_sectors++ = *no_blocks--;
  *no_sectors++ = *no_blocks--;
  *no_sectors++ = *no_blocks--;
  *no_sectors   = *no_blocks--;

  info->flags                       = INFO_FLAG_REMOVABLE;
  info->number_of_cylinders         = 0;
  info->number_of_heads             = 0;
  info->number_of_sectors_per_track = 0;

  return 0;
}