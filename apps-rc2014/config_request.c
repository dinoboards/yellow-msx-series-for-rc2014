#include "v9958.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[4];

uint8_t getVideoMode(void) {
  while (true) {
    printf("NTSC (n) or PAL (p) mode?\r\n");
    fgets(input, sizeof(input), stdin);
    printf("\r\n");

    if (input[0] == 'n' || input[0] == 'N') {
      return NTSC;
    }

    if (input[0] == 'p' || input[0] == 'P') {
      return PAL;
    }
  }
}

uint8_t getLineCount(void) {
  while (true) {
    printf("Vertical dot count 212(2) or 192?\r\n");
    fgets(input, sizeof(input), stdin);
    printf("\r\n");

    if (input[0] == '2') {
      return 212;
    }

    if (input[0] == '1') {
      return 192;
    }
  }
}
