#include "work-area.h"
#include <stdbool.h>
#include <stdlib.h>

uint8_t usb_dev_status(const uint8_t lun) __z88dk_fastcall {
  (void)lun;
  work_area *const p = get_work_area();

  if (!p->ch376.usb_mounted)
    return 0;

  if (!p->ch376.dsk_changed)
    return 1;

  p->ch376.dsk_changed = false;
  return 2;
}