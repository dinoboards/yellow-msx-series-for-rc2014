#include "nextor.h"
#include "ufi.h"
#include <stdlib.h>

uint8_t usb_lun_info(const uint8_t lun, nextor_lun_info *const info) {
  (void)lun;
  _usb_state *const work_area = get_usb_work_area();

  if (work_area->usb_device != USB_IS_FLOPPY)
    return 1;

  ufi_format_capacities_response response;
  const usb_error                result = ufi_capacity(work_area, &response);
  if (result != USB_ERR_OK)
    return 1;

  info->medium_type       = 0; // block_device
  info->sector_size       = response.block_size[1] << 8 + response.block_size[0];
  info->number_of_sectors = response.number_of_blocks[0] << 24 + response.number_of_blocks[1]
                                                         << 16 + response.number_of_blocks[2]
                                                         << 8 + response.number_of_blocks[3];
  info->flags                       = INFO_FLAG_REMOVABLE | INFO_FLAG_FLOPPY;
  info->number_of_cylinders         = 0;
  info->number_of_heads             = 0;
  info->number_of_sectors_per_track = 0;

  return 0;
}