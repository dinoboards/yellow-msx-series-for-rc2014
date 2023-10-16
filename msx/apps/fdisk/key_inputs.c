#include "key_inputs.h"
#include "screen_control.h"
#include <msxdos.h>

char getKey(void) { return msxdosDirio(0xFF); }

uint8_t waitKey(void) {
  uint8_t key;

  while ((key = getKey()) == 0)
    ;
  return key;
}

bool getYesOrNo(void) {
  char key;

  displayCursor();
  key = waitKey() | 32;
  hideCursor();
  return key == 'y';
}
