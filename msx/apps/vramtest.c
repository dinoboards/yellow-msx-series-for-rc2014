#include "v9958.h"
#include <msxdos.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

uint8_t buffer[BUFFER_SIZE];
uint8_t buffer2[BUFFER_SIZE];
uint8_t buffer3[BUFFER_SIZE];

void setTextMode() {
  msxbiosInitxt();
  msxbiosInitPalette();
}

uint8_t conductTest() {
  uint32_t vramAddress = 0x4000;

  vramReadSlow(vramAddress, buffer, BUFFER_SIZE);

  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer2[i] = i;
  }

  vramWriteSlow(vramAddress, buffer2, BUFFER_SIZE);

  vramReadSlow(vramAddress, buffer3, BUFFER_SIZE);

  vramWriteSlow(vramAddress, buffer, BUFFER_SIZE);

  for (int d = 0; d < BUFFER_SIZE; d++)
    if (buffer3[d] != buffer2[d])
      return 1;

  return 0;
}

void main() {
  printf("Testing VRAM\r\n");

  setMode6(212, NTSC);

  for (uint16_t i = 0; i < 800; i++)
    if (conductTest() != 0) {
      setTextMode();
      printf("Failed on %d\r\n", i);
      return;
    }

  setTextMode();
}
