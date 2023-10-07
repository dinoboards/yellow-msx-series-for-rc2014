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
