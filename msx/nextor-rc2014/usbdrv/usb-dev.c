#include "usb-dev.h"
#include "../../apps/libraries/msxdos/msxdos_rc2014_ex.h"
#include "keyboard_drv.h"
// #include "printer_drv.h"
#include "work-area.h"

/**
 * @brief Retrieve hardware probe status of the USB Module
 *
 * @return uint16_t low byte is the number of USB devices present, high byte is 1 if CH376 is present
 */
uint16_t extbio_rc2014_usb_get_present(void) {
  uint8_t l = 0;

  work_area *const p = get_work_area();

  if (!(p->present & PRES_CH376))
    return 0;

  return (uint16_t)p->ch376.count_of_detected_usb_devices | 0x0100;
}

uint16_t rc2014_get_lun_info_ex(const uint8_t device_index, const uint8_t lun, rc2014LunInfoExtended *info) {
  (void)lun;
  const work_area *const p = get_work_area();

  device_config *const dev = get_usb_device_config(device_index);
  if (dev == NULL) {
    info->extendedMediumType = 0;
    info->extendedFlags      = 0;
    return 0;
  }

  info->extendedMediumType                  = dev->type;
  info->extendedFlags                       = 1;
  info->extendedInfo.usbInfo.address        = dev->address;
  info->extendedInfo.usbInfo.configIndex    = 0; // todo need to actually capture this during enumeration and return here
  info->extendedInfo.usbInfo.interfaceIndex = dev->interface_number;

  return 0;
}
