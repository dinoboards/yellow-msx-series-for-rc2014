#include <msxdos.h>
#include <stdio.h>

extern void msxMusicEraseROM();

void main() {
  printf("SPIKE flash MSX-MUSIC ROM ....\r\n");

  msxMusicEraseROM();

  printf("Done\r\n");
}
