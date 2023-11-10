#include "memory.h"
#include <extbio.h>
#include <msxbios/system_vars.h>
#include <msxdos.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static char output[200];

static const char *humanSize(uint32_t bytes) {
  const char *suffix[] = {"B", "KB", "MB"};
  char        length   = sizeof(suffix) / sizeof(suffix[0]);

  uint8_t  i        = 0;
  uint32_t dblBytes = bytes;

  if (bytes > 1024) {
    for (i = 0; (bytes / 1024) > 0 && i < length - 1; i++, bytes /= 1024)
      dblBytes = bytes / 1024;
  }

  sprintf(output, "%ld %s", (int32_t)dblBytes, suffix[i]);
  return output;
}

#define MSXDOS_TPA_BASE_ADDR __at(0x0006)
uint16_t MSXDOS_TPA_BASE_ADDR MSXDOS_TPA_BASE;

memmap_extbio_info info_table;

uint8_t main(const int argc, const char *argv[]) {
  (void)argc;
  (void)argv;

  extbio_get_dev_info_table(4, &info_table);
  memmap_link(info_table.jump_table);

  printf("\r\nRAM Segments:\r\n");
  printf("%6s total RAM\r\n", humanSize((uint32_t)info_table.number_of_segments * 16384L));
  printf("%6s allocated RAM\r\n\n",
         humanSize(((uint32_t)info_table.number_of_segments - (uint32_t)info_table.number_of_free_segments) * 16384L));

  printf("\r\nRAM Working:\r\n");
  printf("  %04X MSXDOS TPA BASE\r\n\n", MSXDOS_TPA_BASE);

  printf("%6u bytes used by bios\r\n", 0x0000 - 0xF380);
  printf("%6u bytes used by system\r\n", 0xF380 - HIMEM + 0x100);

  printf("\r\n%6u bytes TPA available\r\n", MSXDOS_TPA_BASE - 0x100);

  return 0;
}
