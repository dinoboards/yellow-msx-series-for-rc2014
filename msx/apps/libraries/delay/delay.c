#include "delay.h"
#include <system_vars.h>

void delay(uint8_t period) __z88dk_fastcall {
  const int16_t delay_point = ((int16_t)JIFFY) + (period);
  while ((delay_point - ((int16_t)JIFFY) >= 0))
    ;
  ;
}
