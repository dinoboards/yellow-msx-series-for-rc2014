#define __Z88DK_R2L_CALLING_CONVENTION
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "v9958.h"

uint8_t getRandomSeed() __naked __z88dk_fastcall {
  __asm
  ld  A, R
  LD  L, A
  RET
  __endasm;
}

extern void cleanexit();

void main() {
  srand(getRandomSeed());
  setMode6(212, PAL);
  setPalette();
  clearScreenBank0(4);
  uint8_t c = rand() & 15;

  for(unsigned int i = 0; i < 40000; i++) {
    pointSet(rand() % 512, i % 212, rand() & 15, CMD_LOGIC_IMP);
    // drawLine(rand() % 512, i % 212, rand() % 512, i % 212, rand() & 15, CMD_LOGIC_IMP);
  }

  cleanexit();
}
