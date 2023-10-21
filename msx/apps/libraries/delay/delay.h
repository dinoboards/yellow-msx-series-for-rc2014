#ifndef __DELAY
#define __DELAY

#include <stdlib.h>
#include <system_vars.h>

extern void delay(const uint8_t period) __z88dk_fastcall;
extern void long_delay(const int16_t milliseconds) __z88dk_fastcall;
extern void delay_20ms(void);
extern void delay_short(void);
extern void delay_medium(void);

#define from_ms_60hz(period)    ((int16_t)(1L * (period) * (60) / 1000))
#define from_ms_50hz(period)    ((int16_t)(1L * (period) * (50) / 1000))
#define from_ms(period)         ((int16_t)(1L * (period) * (GET_VDP_FREQUENCY()) / 1000))
#define from_seconds(period)    ((int16_t)(1L * (period) * (GET_VDP_FREQUENCY())))
#define get_future_time(period) ((int16_t)(((int16_t)JIFFY) + (period)))
#define is_time_past(ft)        ((ft - ((int16_t)JIFFY) < 0))

#endif
