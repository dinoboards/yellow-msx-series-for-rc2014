#include "fossil.h"
#include "msxdos.h"
#include "xstdio.h"
#include <extbio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <system_vars.h>

bool hasRC2014;

const char *extendedBiosName(uint8_t id) {
  switch (id) {
  case 0:
    return "Broadcast";

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

  case 214:
    hasRC2014 = true;
    return "RC2014 Extensions";

  case 241:
    return "MultiMente";

  default:
    return "Unknown";
  }
}

extbio_device_table table[32];

extern uint8_t getSlotPage0(void *) __z88dk_fastcall;

void main(void) {
  hasRC2014 = false;

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

  const uint8_t slt = getSlotPage0(table);

  printf("SLOT %02X for address %p\r\n", slt, table);

  const uint8_t count = extbio_get_dev(table) - table;

  for (int i = 0; i < count; i++)
    printf("%d: %s (%02X) (%02X)\r\n", i, extendedBiosName(table[i].deviceId), table[i].deviceId, table[i]._reserved);

  const uint16_t reduced_mapper_table = 0;
  const uint8_t  number               = 0;

  if (hasRC2014) {
    const uint8_t v = extbio_rc2014_get_version();
    printf("RC2014 Extensions Version: %d\r\n", v);

    const uint16_t h = extbio_rc2014_hello();
    printf("RC2014 Hello World Test: %d\r\n", h);
  }

  const jump_table_entry *hl = unapi_get_ram_helper(&reduced_mapper_table, &number);
  printf("\r\nUNAPI:\r\n");
  printf("  RAM HELPER\r\n");
  printf("    JUMP TABLE @ %p\r\n", hl);
  printf("    REDUCED TABLE @ %p\r\n", reduced_mapper_table);
  printf("    NUMBER OF ENTRIES %d\r\n", number);

  printf("\r\n");
}
