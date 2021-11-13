#define __Z88DK_R2L_CALLING_CONVENTION
#include "config_request.h"
#include "v9958.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

uint8_t getRandomSeed() __naked __z88dk_fastcall {
  // clang-format off
  __asm
  ld  A, R
  LD  L, A
  RET
  __endasm;
  // clang-format on
}
extern void cleanexit();

RGB palette[16] = {
    {0, 0, 0}, {1, 0, 0}, {4, 0, 0}, {4, 1, 1}, {15, 0, 0}, {0, 1, 0}, {0, 4, 0}, {1, 4, 1}, {1, 8, 1}, {0, 0, 1}, {0, 0, 4}, {1, 1, 4}, {1, 1, 8}, {10, 0, 10}, {0, 15, 15}, {15, 15, 15},
};

void main() {
  const uint8_t mode = getVideoMode();
  const uint8_t lines = getLineCount();

  srand(getRandomSeed());
  setMode6(lines, mode);
  setPalette(palette);
  clearScreenBank0(4);
  uint8_t c = rand() & 15;

  for (unsigned int i = 0; i < 4000; i++) {
    drawLine(rand() % 512, i % lines, rand() % 512, i % lines, rand() & 15, CMD_LOGIC_IMP);
  }

  while (!kbhit())
    ;

  cleanexit();
}
