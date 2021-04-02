#define __Z88DK_R2L_CALLING_CONVENTION
#include "msxdos.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void    extbio_get_dev(uint8_t *table) __z88dk_fastcall;
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

uint8_t table[32];

typedef struct {
  uint8_t slot_id;
  void *  jump_table;
  uint8_t reserved;
} extbio_info;

extbio_info info_table[4];

extern void get_dev_info_table(uint8_t device_id, extbio_info *info_table);

void main() {
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

  extbio_get_dev(table);

  for (int i = 0; i < 32 && table[i]; i++)
    printf("%d: %s (%02X)\r\n", i, extendedBiosName(table[i]), table[i]);

  printf("\r\n");

  /*** GET RS232 SERIAL SLOT NUMBERS ***/

  get_dev_info_table(8, info_table);

  printf("Slot ID %02X, Address %p\r\n", info_table[0].slot_id, info_table[0].jump_table);
}
