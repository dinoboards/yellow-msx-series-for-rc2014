#include "screen_control.h"
#include <msxbios.h>
#include <stdio.h>
#include <string.h>
#include <system_vars.h>

screenConfiguration        currentScreenConfig;
static screenConfiguration originalScreenConfig;
uint8_t                    screenLinesCount;

void saveOriginalScreenConfiguration(void) {
  originalScreenConfig.screenMode          = SCRMOD;
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

void locateX(uint8_t x) __sdcccall(1) { msxbiosPosit(x + 1, CSRY); }

void locate(uint8_t x, uint8_t y) __sdcccall(1) { msxbiosPosit(x + 1, y + 1); }

void printCentered(char *string) __sdcccall(1) {
  uint8_t pos = (currentScreenConfig.screenWidth - strlen(string)) / 2;
  homeCursor();
  locateX(pos);
  printf(string);
}
