#ifndef __DELAY
#define __DELAY

#include <stdlib.h>
#include <system_vars.h>

extern void delay(uint8_t period) __z88dk_fastcall;

#define from_ms(period)         ((int16_t)(1L * (period) * (GET_VDP_FREQUENCY()) / 1000))
#define get_future_time(period) ((int16_t)(((int16_t)JIFFY) + (period)))
#define is_time_past(ft)        ((ft - ((int16_t)JIFFY) < 0))

#endif
