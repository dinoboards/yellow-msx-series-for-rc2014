#include "screen_control.h"
#include <stdio.h>
#include <system_vars.h>

screenConfiguration        currentScreenConfig;
static screenConfiguration originalScreenConfig;
uint8_t                    screenLinesCount;

void saveOriginalScreenConfiguration(void) {
  originalScreenConfig.screenMode          = *(uint8_t *)SCRMOD;
  originalScreenConfig.screenWidth         = LINLEN;
  originalScreenConfig.functionKeysVisible = (*(uint8_t *)CNSDFG != 0);
}

void composeWorkScreenConfiguration(void) {
  currentScreenConfig.screenMode          = 0;
  currentScreenConfig.screenWidth         = 80;
  currentScreenConfig.functionKeysVisible = false;
  screenLinesCount                        = *(uint8_t *)CRTCNT;
}

void setScreenConfiguration(void) {
  LINL40 = 80;
  msxbiosInitxt();
}

void locateX(uint8_t x) { msxbiosPosit(x + 1, CSRY); }

void locate(uint8_t x, uint8_t y) { msxbiosPosit(x + 1, y + 1); }

void printCentered(char *string) {
  uint8_t pos = (currentScreenConfig.screenWidth - strlen(string)) / 2;
  homeCursor();
  locateX(pos);
  printf(string);
}
