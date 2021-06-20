#define __Z88DK_R2L_CALLING_CONVENTION
#include "msxdos.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extbio.h"
#include "telnet/fossil_interface.h"
#include "xstdio.h"

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
    return "RC2014 Extensions";

  case 241:
    return "MultiMente";

  default:
    return "Unknown";
  }
}

extbio_device_table table[32];

extern uint8_t getSlotPage0(void*) __z88dk_fastcall;

void main() {
  const bool extendedBiosReady = HOKVLD & 1;
  if (!extendedBiosReady) {
    xprintf("No bios extenstions installed\r\n");
    return;
  }

  const bool atLeastOneInstalled = EXTBIO[0] != RET_INSTRUCTION;
  if (!atLeastOneInstalled) {
    xprintf("No bios extensions installed\r\n");
    return;
  }

  xprintf("\r\nInstalled Extended Bios Systems:\r\n");
  xprintf("-----------------------------------\r\n");

  const uint8_t slt = getSlotPage0(table);

  xprintf("SLOT %02X for address %p\r\n", slt, table);

  const uint8_t count = extbio_get_dev(table) - table;

  for (int i = 0; i < count; i++)
    xprintf("%d: %s (%02X) (%02X)\r\n", i, extendedBiosName(table[i].deviceId), table[i].deviceId, table[i]._reserved);

  xprintf("\r\n");
}
