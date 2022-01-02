#include "arguments.h"
#include <msxdos.h>
#include <stdio.h>

extern void msxMusicEraseROM();
extern void msxMusicWrite4KPage(uint8_t page, uint8_t buffer[4096]);  // Page is 0 to 127 - to address a 4K block within the 512K ROM address range.
extern bool msxMusicVerify4KPage(uint8_t page, uint8_t buffer[4096]); // Page is 0 to 127 - to address a 4K block within the 512K ROM address range.

extern uint16_t msxMusicSetSlot();
extern void     msxMusicRestoreSlot(uint16_t slot_assignment) __z88dk_fastcall;
extern uint16_t spike();
extern void     msxMusicSetPage(uint8_t p) __z88dk_fastcall;

uint8_t __at 0x8000 buffer[1024 * 16];

static uint8_t *ptr;
static uint16_t slot_assignment;
static FILE *   pFile;
static size_t   length;

void main(const int argc, const unsigned char **argv) {

  process_cli_arguments(argc, argv);

  pFile = fopen(pFileName, "rb");

  if (!pFile) {
    printf("Unable to open file '%s'\r\n", pFileName);
    exit(1);
  }

  length = fread(buffer, 1, 16 * 1024, pFile);

  fclose(pFile);

  printf("Flashing %lu bytes to MSX-MUSIC onboard ROM ", (unsigned long)length);

  msxMusicEraseROM();

  for (uint8_t i = 0; i <= 3; i++) {
    uint8_t *p = &buffer[i * 0x1000];
    msxMusicWrite4KPage(i, p);
    if (!msxMusicVerify4KPage(i, p))
      goto error;

    printf(".");
  }

  printf("\r\nFlash completed.  Please reset now.\r\n");
  return;

error:
  printf("\r\nVerification failure - aborting\r\n");
  exit(1);
}
