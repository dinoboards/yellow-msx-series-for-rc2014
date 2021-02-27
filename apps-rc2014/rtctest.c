#include "cpm.h"
#include "rp5c01.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main() {
  printf("TODO: Test the RP5C01 chip\r\n");

  const uint8_t detected = rp5c01Detect();

  if (detected)
    printf("RP5C01 detected\r\n");
  else {
    printf("RP5C01 not detected\r\n");
    exit(1);
  }

  const uint8_t errorCode = rp5c01SetByte(0, 0x48);
  if (errorCode) {
    printf("rp5c01SetByte return error code %02X\r\n", errorCode);
    exit(1);
  }
  const uint16_t data = rp5c01GetByte(0);

  if (data != 0x48)
    printf("NVRAM failure - wrote 0x48, got %02X returned (error code: %02X).\r\n", data & 255, data > 8);
  else
    printf("NVRAM success for index 1\r\n");
}
