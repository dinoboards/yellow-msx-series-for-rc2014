#define __Z88DK_R2L_CALLING_CONVENTION
#include "msxdos.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void    getdev();
extern void    getsystem();
extern uint8_t table[32];

const char *extendedBiosName(uint8_t id) {
  switch (id) {
  case 0:
    return "Broadcase";

  case 4:
    return "MSX-DOS Memory Mapper";

  case 8:
    return "MSX-Modem & RS-232C";

  case 10:
    return "MSX-AUDIO";

  case 16:
    return "MSX-JE";

  case 17:
    return "Kanji driver";

  case 34:
    return "MSX-UNAPI";

  case 77:
    return "MemMan";

  case 132:
    return "μ-driver by Yoshikazu Yamamoto";

  case 241:
    return "MultiMente";

  default:
    return "Unknown";
  }
}

void main() {
  printf("This is a test\r\n");

  const bool extendedBiosReady = HOKVLD & 1;
  if (!extendedBiosReady) {
    printf("No bios extenstions installed\r\n");
    return;
  }

  const bool atLeastOneInstalled = EXTBIO[0] != RET_INSTRUCTION;
  if (!atLeastOneInstalled) {
    printf("No bios extensions installed\r\n");
    return;
  }

  printf("\r\nInstalled Extended Bios Systems:\r\n");
  printf("-----------------------------------\r\n");

  getdev();

  for (int i = 0; i < 32 && table[i]; i++)
    printf("%d: %s (%02X)\r\n", i, extendedBiosName(table[i]), table[i]);

  printf("\r\n");
}
