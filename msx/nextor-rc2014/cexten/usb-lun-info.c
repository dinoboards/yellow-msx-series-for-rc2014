#include "ch376.h"
#include "print.h"
#include "scsi.h"
#include "work-area.h"
#include <stdlib.h>

#define INFO_FLAG_REMOVABLE 0x01
#define INFO_FLAG_READ_ONLY 0x02
#define INFO_FLAG_FLOPPY    0x04

typedef struct _nextor_lun_info {
  uint8_t  medium_type;
  uint16_t sector_size;
  uint32_t number_of_sectors;
  uint8_t  flags;
  uint16_t number_of_cylinders;
  uint8_t  number_of_heads;
  uint8_t  number_of_sectors_per_track;
} nextor_lun_info;

uint32_t usb_scsi_read_capacity() {
  work_area *const p = get_work_area();

  uint8_t result;
  if ((result = do_scsi_cmd(&p->ch376,
                            p->ch376.storage_device_info.device_address,
                            0,
                            sizeof(_scsi_read_capacity),
                            8,
                            (uint8_t *)scsi_read_capacity,
                            p->ch376.scsi_device_info.buffer,
                            false)) != CH_USB_INT_SUCCESS) {
    return 0;
  }

  const uint32_t capacity = *((uint32_t *)p->ch376.scsi_device_info.buffer);

  return ((capacity >> 24) & 0xff) |      // move byte 3 to byte 0
         ((capacity << 8) & 0xff0000) |   // move byte 1 to byte 2
         ((capacity >> 8) & 0xff00) |     // move byte 2 to byte 1
         ((capacity << 24) & 0xff000000); // byte 0 to byte 3
}

uint8_t usb_lun_info(const uint8_t lun, nextor_lun_info *const info) {
  if (lun != 1)
    return 1;

  const uint32_t capacity = usb_scsi_read_capacity();

  info->medium_type                 = 0; // block_device
  info->sector_size                 = 512;
  info->number_of_sectors           = capacity;
  info->flags                       = INFO_FLAG_FLOPPY;
  info->number_of_cylinders         = 0;
  info->number_of_heads             = 0;
  info->number_of_sectors_per_track = 0;

  return 0;
}