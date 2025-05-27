#include <stdio.h>
#include <stdint.h>

// clang-format off
#define DI        \
        __asm     \
        DI        \
        __endasm

#define EI        \
        __asm     \
        EI        \
        __endasm
// clang-format on

__sfr __at(0xA0) AYSEL;
__sfr __at(0xA1) AY_RDAT;
__sfr __at(0xA2) AY_RIN;

uint8_t main(const int argc, const char *argv[]) {
  (void)argc;
  (void)argv;

  DI;
  AYSEL=3;
  AY_RDAT=255;
  const uint8_t response = AY_RIN;
  EI;

  if (response == 255)
    printf("GAME: YM2149F\r\n");
  else
    printf("GAME: AY-3-8910\r\n");

  return 0;
}
