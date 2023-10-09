#include "delay.h"
#include <system_vars.h>

void delay(const uint8_t period) __z88dk_fastcall {
  __asm__("EI");
  const int16_t delay_point = ((int16_t)JIFFY) + (period);
  while ((delay_point - ((int16_t)JIFFY) >= 0))
    ;
  ;
}

void long_delay(const int16_t seconds) __z88dk_fastcall {
  __asm__("EI");
  const int16_t ft = get_future_time(from_seconds(seconds));
  while (!is_time_past(ft))
    ;
}

void delay_20ms(void) { delay(1); }

void delay_short(void) { delay(3); }

void delay_medium(void) { delay(30); }
