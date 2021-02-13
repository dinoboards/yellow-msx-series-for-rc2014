#include "cpm.h"
#include "ym2149.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t oldd[2] = {0, 0};
uint8_t oldb1[2] = {0, 0};
uint8_t oldb2[2] = {0, 0};

const char *direction(const uint8_t dir) {
  switch (dir) {
  case 1:
    return "UP";

  case 2:
    return "UP-RIGHT";

  case 3:
    return "RIGHT";

  case 4:
    return "DOWN-RIGHT";

  case 5:
    return "DOWN";

  case 6:
    return "DOWN-LEFT";

  case 7:
    return "LEFT";

  case 8:
    return "UP-LEFT";

  default:
    return "UNKOWN";
  }
}

const char *button(const uint8_t b) { return b == 1 ? "PRESSED" : "RELEASED"; }

void logController(const uint8_t i) {
  const uint8_t d = getControllerDirection(i == 0 ? 1 : 2);
  const uint8_t b1 = getControllerButton(i == 0 ? 1 : 2);
  const uint8_t b2 = getControllerButton(i == 0 ? 3 : 4);

  if (oldd[i] != d && d != 0)
    printf("Controller %d: %s\r\n", i + 1, direction(d));

  if (oldb1[i] != b1)
    printf("Controller %d: Button 1: %s\r\n", i + 1, button(b1));

  if (oldb2[i] != b2)
    printf("Controller %d: Button 2: %s\r\n", i + 1, button(b2));

  oldd[i] = d;
  oldb1[i] = b1;
  oldb2[i] = b2;
}

void main() {
  const bool ym2149Found = ym2149Probe();

  if (!ym2149Found) {
    printf("Failed to find ym2149 audio chip\r\n");
    return;
  }

  printf("Press buttons on your game pad controller now\r\n");
  printf("Press any key, on keyboard, to exit\r\n");

  while (cRawIo() == 0) {
    logController(0);
    logController(1);
  }
}
