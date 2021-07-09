#include "serial.h"
#include "msx.h"

#define VDP_FREQUENCY 50

bool waitForByte(uint8_t period) __z88dk_fastcall {
  const int16_t timeout = JIFFY + period;

  while (JIFFY < timeout && !fossil_rs_in_stat())
    ;

  return fossil_rs_in_stat();
}

int _inbyte(uint8_t period) __z88dk_fastcall {
  const int16_t timeout = JIFFY + period;

  while (JIFFY < timeout && !fossil_rs_in_stat())
    ;

  int r = 0;
  if (fossil_rs_in_stat()) {
    r = fossil_rs_in();
    r = r & 0x00FF;
  } else
    r = -99;

  return r;
}
