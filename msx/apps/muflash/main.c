#include "arguments.h"
#include <conio.h>
#include <msxdos.h>
#include <stdio.h>

extern void msx_music_erase_ROM();
extern void msx_music_write_4K_page(const uint8_t page, const uint8_t buffer[4096]);  // Page is 0 to 127 - to address a 4K block within the 512K ROM address range.
extern bool msx_music_verify_4K_page(const uint8_t page, const uint8_t buffer[4096]); // Page is 0 to 127 - to address a 4K block within the 512K ROM address range.

uint8_t __at 0x8000 buffer[4096]; // Must not be within page1 (0x4000-0x7FFF) as that is where the ROM will be mapped.

static uint16_t slot_assignment;
static FILE *   file;

const uint8_t progress_bar_length = 20;

void exit_cleanup() { fclose(file); }

void main(const int argc, const unsigned char **argv) {

  process_cli_arguments(argc, argv);

  file = fopen(flash_file_name, "rb");

  if (!file) {
    printf("Unable to open file '%s'\r\n", flash_file_name);
    exit(1);
  }

  atexit(exit_cleanup);

  fseek(file, 0L, SEEK_END);
  const unsigned long sz = ftell(file);
  const uint8_t       number_of_pages = (sz / 4096) + 1;

  printf("Flashing %lu bytes (%d 4K blocks) to MSX-MUSIC onboard ROM.  Proceed (Y/n)\r\n", sz, number_of_pages);
  const char ch = getch();

  if (ch != 'Y')
    return;

  msx_music_erase_ROM();

  rewind(file);

  unsigned long length = fread(buffer, 1, 4096, file);

  uint8_t page_index = 0;
  printf("\r\n");

  while (length > 0) {
    msx_music_write_4K_page(page_index, buffer);
    if (!msx_music_verify_4K_page(page_index, buffer))
      goto error;
    page_index++;

    uint8_t completed_range = page_index * progress_bar_length / number_of_pages;

    printf("\r\x15");
    printf("[");
    uint8_t x = 0;
    for (; x < completed_range; x++)
      printf(".");
    for (; x < progress_bar_length; x++)
      printf(" ");
    printf("] %dKB written", (page_index * 4));

    length = fread(buffer, 1, 4096, file);
  }

  printf("\r\n\r\nFlash completed.  Please reset now.\r\n");

  return;

error:
  printf("\r\nVerification failure - aborting\r\n");
  exit(1);
}
