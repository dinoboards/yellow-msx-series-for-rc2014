#include "serial.h"
#include "msx.h"
#include <stdio.h>

bool wait_for_byte(uint8_t period) __z88dk_fastcall {
  const int16_t timeout = ((int16_t)JIFFY) + period;

  while ((timeout - ((int16_t)JIFFY) >= 0) && !fossil_rs_in_stat())
    ;

  return fossil_rs_in_stat();
}
