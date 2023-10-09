#include "cpm.h"
#include <stdio.h>

extern uint8_t scanRow(const uint8_t rowIndex) __z88dk_fastcall;
extern void    initPPI(void);

uint8_t keyPatterns[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void printBinary(uint8_t n) {
  for (uint8_t i = 0; i < 8; i++) {
    printf(n & 0x80 ? "1" : "0");

    n <<= 1;
  }
}

void main(void) {
  printf("Testing MSX keyboard interface\r\n");
  printf("As you press down each key, you should see the corresponding row and column bit go to 0.  Upon releasing key, the bit "
         "should return to 1\r\n");
  printf("Scanning key matrix rows/columns....\r\n");
  printf("Press any key thru serial to cancel\r\n");

  initPPI();

  while (cRawIo() == 0) {
    for (uint8_t i = 0; i <= 8; i++) {
      const uint8_t pattern = scanRow(i);

      if (keyPatterns[i] != pattern) {
        keyPatterns[i] = pattern;
        printf("Row %d = ", i);
        printBinary(pattern);
        printf("\r\n");
      }
    }
  }
}
