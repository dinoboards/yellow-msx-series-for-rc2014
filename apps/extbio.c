#define __Z88DK_R2L_CALLING_CONVENTION
#include "msxdos.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void    getdev();
extern void    getsystem();
extern uint8_t table[32];

void main() {
  printf("This is a test\r\n");

  const bool extendedBiosReady = HOKVLD & 1;
  printf("extendedBiosReady: %s\r\n", extendedBiosReady ? "Installed" : "Not Installed");

  const bool atLeastOneInstalled = EXTBIO[0] != RET_INSTRUCTION;
  printf(atLeastOneInstalled ? "At least one installed\r\n" : "Null Installation\r\n");

  getdev();
  for (int i = 0; i < 32; i++)
    printf("%02X ", table[i]);
  printf("\r\n");

  // getsystem();
  // for(int i = 0; i < 32; i++)
  //   printf("%02X ", table[i]);
  // printf("\r\n");
}
