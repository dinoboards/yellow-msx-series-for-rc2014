#define __Z88DK_R2L_CALLING_CONVENTION
#include "msxdos.h"
#include "v9958.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  srand(getRandomSeed());
  setMode6(212, PAL);
  // setPalette(palette);
  clearScreenBank0(4);
  uint8_t c = rand() & 15;

  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t offset = 0;
  uint8_t  i = 0;

  while (true) {
    if (msxdosDirio(0xFF) != 0)
      break;

    pointSet(x, y, i++ & 15, CMD_LOGIC_IMP);

    x++;
    x %= 512;

    if (x == 0) {
      y += 4;
      if (y >= 212) {
        offset++;
        y = offset;
      }
      y %= 212;
    }
  }

  cleanexit();
}
