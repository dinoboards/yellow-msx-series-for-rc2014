#include <stdio.h>
#include <string.h>

typedef struct {
  uint8_t bModifierKeys;
  uint8_t bReserved;
  uint8_t keyCode1;
  uint8_t keyCode2;
  uint8_t keyCode3;
  uint8_t keyCode4;
  uint8_t keyCode5;
  uint8_t keyCode6;
} keyboard_report;

keyboard_report current;

#define ESC 0x1B

char scancodes_table[128] = {
    0x00, /*Reserved*/
    0x00, /*Reserved*/
    0x00, /*Reserved*/
    0x00, /*Reserved*/

    /* 0x04 */
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',

    /* 0x0C */
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',

    /* 0x14 */
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',

    /* 0x1C */
    'y',
    'z',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',

    /* 0x24 */
    '7',
    '8',
    '9',
    '0',
    '\r',
    ESC,
    '\b',
    '\t',

    /* 0x2C */
    ' ',
    '-',
    '=',
    '[',
    ']',
    '\\',
    '#',
    ';',

    /* 0x34 */
    '\'',
    '`',
    ',',
    '.',
    '/',
    0x00 /*CAPSLOCK*/,
    0x00 /* F1 */,
    0x00 /* F2 */,

    /* 0x3C */
    0x00 /* F3 */,
    0x00 /* F4 */,
    0x00 /* F5 */,
    0x00 /* F6 */,
    0x00 /* F7 */,
    0x00 /* F8 */,
    0x00 /* F9 */,
    0x00 /* F10 */,

    /* 0x44 */
    0x00 /* F11 */,
    0x00 /* F12 */,
    0x00 /* PRINTSCREEN */,
    0x00 /* SCROLLLOCK */,
    0x00 /* PAUSE */,
    0x00 /* INSERT */,
    0x00 /* HOME */,
    0x00 /* PAGEUP */,

    /* 0x4C */
    0x00 /* DELETE */,
    0x00 /* END */,
    0x00 /* PAGEDOWN */,
    0x00 /* RIGHT */,
    0x00 /* LEFT */,
    0x00 /* DOWN */,
    0x00 /* UP */,
    0x00 /* NUMLOCK */,

    /* 0x54 */
    '/' /* KP / */,
    '*' /* KP * */,
    '-' /* KP - */,
    '+' /* KP + */,
    '\r' /* KP ENTER */,
    '1' /* KP 1 */,
    '2' /* KP 2 */,
    '3' /* KP 3 */,

    /* 0x5C */
    '4' /* KP 4 */,
    '5' /* KP 5 */,
    '6' /* KP 6 */,
    '7' /* KP 7 */,
    '8' /* KP 8 */,
    '9' /* KP 9 */,
    '0' /* KP 0 */,
    '.' /* KP . */,

    /* 0x64 */
    '\\',
    0x00 /* MENU */,
    0x00 /* POWER */,
    '=' /* KP = */,
    0x00 /* F13 */,
    0x00 /* F14 */,
    0x00 /* F15 */,
    0x00 /* F16 */,

    /* 0x6C */
    0x00 /* F17 */,
    0x00 /* F18 */,
    0x00 /* F19 */,
    0x00 /* F20 */,
    0x00 /* F21 */,
    0x00 /* F22 */,
    0x00 /* F23 */,
    0x00 /* F24 */,

    /* 0x74 */
    0x00 /* EXECUTE */,
    0x00 /* HELP */,
    0x00 /* MENU */,
    0x00 /* SELECT */,
    0x00 /* STOP */,
    0x00 /* AGAIN */,
    0x00 /* UNDO */,
    0x00 /* CUT */,

    /* 0x7C */
    0x00 /* COPY */,
    0x00 /* PASTE */,
    0x00 /* FIND */,
    0x00 /* MUTE */,
};

char scancode_to_char(void) {

  if (current.keyCode1 >= 0x80)
    return 0;

  return scancodes_table[current.keyCode1];
}

void main(const int argc, const char *argv[]) {
  (void)argc;
  (void)argv;
  printf("     Hello World\r\n");

  memset(&current, 0, sizeof(current));

  current.keyCode1 = 4;

  printf("     %c\r\n", scancode_to_char());
}
