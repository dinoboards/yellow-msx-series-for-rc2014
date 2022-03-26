#include "nextor.h"
#include <stdlib.h>

uint8_t usb_lun_info(const uint8_t lun, nextor_lun_info *const info) {
  (void)lun;

  info->medium_type                 = 0; // block_device
  info->sector_size                 = 512;
  info->number_of_sectors           = 720L * 1025 / 512;
  info->flags                       = INFO_FLAG_REMOVABLE;
  info->number_of_cylinders         = 0;
  info->number_of_heads             = 0;
  info->number_of_sectors_per_track = 0;

  return 1;
}