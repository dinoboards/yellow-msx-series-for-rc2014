#ifndef __CLASS_HID_KEYBOARD_H__
#define __CLASS_HID_KEYBOARD_H__

#include <stdint.h>

typedef struct {
  uint8_t bModifierKeys;
  uint8_t bReserved;
  uint8_t keyCode[6];
} keyboard_report;

extern char scancodes_table[128];
extern char scancode_to_char(const uint8_t code) __sdcccall(1);

#endif
