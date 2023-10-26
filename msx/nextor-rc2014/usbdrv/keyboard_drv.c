#include "keyboard_drv.h"
#include <class_hid.h>
#include <class_hid_keyboard.h>
#include <stdbool.h>
#include <stdint.h>
#include <system_vars.h>
#include <usb_state.h>
#include <z80.h>

void key_put_into_buf(const uint8_t code) __z88dk_fastcall {
  char *buffer_put = PUTPNT;

  *buffer_put++ = code;

  const uint8_t x = ((uint16_t)buffer_put & 0xFF);
  if (x == 0x18)
    buffer_put = KEYBUF;

  if (buffer_put != GETPNT)
    PUTPNT = buffer_put;
}

void drv_timi_keyboard(void) {
  device_config_keyboard *const keyboard_config = (device_config_keyboard *)find_device_config(USB_IS_KEYBOARD);
  if (keyboard_config == NULL)
    return;

  _usb_state *const p = get_usb_work_area();
  if (p->active)
    return;

  p->active = true;

  keyboard_report report;

  ch_configure_nak_retry_disable();
  const usb_error result = usbdev_data_in_transfer_ep0((device_config *)keyboard_config, (uint8_t *)report, 8);
  ch_configure_nak_retry_3s();
  if (result == 0) {
    const char c = scancode_to_char(report.keyCode[0]);
    key_put_into_buf(c);
  }

  p->active = false;
}

void install_keyboard(void) {
  device_config_keyboard *const keyboard_config = (device_config_keyboard *)find_device_config(USB_IS_KEYBOARD);

  const bool hasKeyboard = keyboard_config != NULL;

  if (!hasKeyboard)
    return;

  hid_set_protocol(keyboard_config, 1);
  hid_set_idle(keyboard_config, 0x80);
}
