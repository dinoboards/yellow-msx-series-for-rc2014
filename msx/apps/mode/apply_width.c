#include "apply_width.h"
#include "argument_width.h"
#include <msxbios/msxbios.h>
#include <msxbios/system_vars.h>

#include <stdio.h>

#define RTC_BLOCK_2 0b00100000
#define RTC_REG_4   0b00000100
#define RTC_REG_5   0b00000101

uint8_t apply_width_settings(void) {
  if (col_width >= 33) {
    LINL40 = col_width;
    msxbiosInitxt();
  } else {
    LINL32 = col_width;
    msxbiosInit32();
  }

  msxbiosWrtClk(RTC_BLOCK_2 | RTC_REG_4, col_width & 0x0F);
  msxbiosWrtClk(RTC_BLOCK_2 | RTC_REG_5, (col_width >> 4) & 0x07);

  return 0;
}
