#include "work-area.h"

_usb_state *get_usb_work_area() {
  work_area *const p = get_work_area();
  return &p->ch376;
}

uint8_t get_number_of_usb_drives() {
  const work_area *const p = get_work_area();
  uint8_t                c = 0;

  const device_config *const last = &p->ch376.storage_device[MAX_NUMBER_OF_STORAGE_DEVICES - 1];
  for (device_config *dev = p->ch376.storage_device; dev <= last; dev++) {
    if (dev->address != 0)
      c++;
  }
  return c;
}

#define PRES_CF   1  /* BIT MASK FOR COMPACTFLASH PRESENT */
#define PRES_MS   2  /* BIT MASK FOR MSX MUSIC NOR FLASH PRESENT */
#define PRES_USB1 4  /* BIT MASK FOR USB1 STORAGE PRESENT AT BOOT UP */
#define PRES_USB2 8  /* BIT MASK FOR USB2 STORAGE PRESENT AT BOOT UP */
#define PRES_USB3 16 /* BIT MASK FOR USB3 STORAGE PRESENT AT BOOT UP */
#define PRES_USB4 32 /* BIT MASK FOR USB4 STORAGE PRESENT AT BOOT UP */

#define BIT_PRES_CF   0 /* BIT POSTION FOR COMPACTFLASH PRESENT */
#define BIT_PRES_MS   1 /* BIT POSTION FOR MSX MUSIC NOR FLASH PRESENT */
#define BIT_PRES_USB1 2 /* BIT POSTION FOR USB1 STORAGE PRESENT */
#define BIT_PRES_USB2 3 /* BIT POSTION FOR USB2 STORAGE PRESENT */
#define BIT_PRES_USB3 4 /* BIT POSTION FOR USB3 STORAGE PRESENT */
#define BIT_PRES_USB4 5 /* BIT POSTION FOR USB4 STORAGE PRESENT */

typedef enum {
  DEV_MAP_NONE = 0,
  DEV_MAP_ROM  = 1,
  DEV_MAP_CF   = 2,
  DEV_MAP_MS   = 3,
  DEV_MAP_USB1 = 4,
  DEV_MAP_USB2 = 5,
  DEV_MAP_USB3 = 6,
  DEV_MAP_USB4 = 7
} device_map;

const uint8_t mappings[20][7] = {{DEV_MAP_ROM, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_CF, DEV_MAP_ROM, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_ROM, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_CF, DEV_MAP_ROM, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_USB2, DEV_MAP_ROM, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_USB2, DEV_MAP_CF, DEV_MAP_ROM, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_USB2, DEV_MAP_USB3, DEV_MAP_ROM, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_USB2, DEV_MAP_USB3, DEV_MAP_CF, DEV_MAP_ROM, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_USB2, DEV_MAP_USB3, DEV_MAP_USB4, DEV_MAP_ROM, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_USB2, DEV_MAP_USB3, DEV_MAP_USB4, DEV_MAP_CF, DEV_MAP_ROM, DEV_MAP_NONE},

                                 {DEV_MAP_ROM, DEV_MAP_MS, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_CF, DEV_MAP_ROM, DEV_MAP_MS, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_ROM, DEV_MAP_MS, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_CF, DEV_MAP_ROM, DEV_MAP_MS, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_USB2, DEV_MAP_ROM, DEV_MAP_MS, DEV_MAP_NONE, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_USB2, DEV_MAP_CF, DEV_MAP_ROM, DEV_MAP_MS, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_USB2, DEV_MAP_USB3, DEV_MAP_ROM, DEV_MAP_MS, DEV_MAP_NONE, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_USB2, DEV_MAP_USB3, DEV_MAP_CF, DEV_MAP_ROM, DEV_MAP_MS, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_USB2, DEV_MAP_USB3, DEV_MAP_USB4, DEV_MAP_ROM, DEV_MAP_MS, DEV_MAP_NONE},
                                 {DEV_MAP_USB1, DEV_MAP_USB2, DEV_MAP_USB3, DEV_MAP_USB4, DEV_MAP_CF, DEV_MAP_ROM, DEV_MAP_MS}

};

/**
 * @brief Convert a device index (1-7) to a device identity (USB, CompactFlash, etc)
 *
 * @param device_index number of the msx-dos device (1-7)
 * @return device_map the identified device
 */
device_map device_mapping(const uint8_t device_index) __z88dk_fastcall {
  const uint8_t present_mask = get_work_area()->present;

  uint8_t usb_index = 0;
  if (present_mask & PRES_USB1)
    usb_index++;

  if (present_mask & PRES_USB2)
    usb_index++;

  if (present_mask & PRES_USB3)
    usb_index++;

  if (present_mask & PRES_USB4)
    usb_index++;

  usb_index <<= 1;

  if (present_mask & PRES_CF)
    usb_index++;

  if (present_mask & PRES_MS)
    usb_index += 10;

  return mappings[usb_index][device_index - 1];
}
