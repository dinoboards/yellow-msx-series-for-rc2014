#ifndef __RP5C01
#define __RP5C01

#include <stdbool.h>
#include <stdlib.h>

extern uint8_t  rp5c01Detect();
extern uint16_t rp5c01GetByte(uint8_t index) __z88dk_fastcall;
extern uint8_t  _rp5c01SetByte(uint16_t r) __z88dk_fastcall;
extern void     rp5c01TestMode(uint8_t testBits) __z88dk_fastcall;

extern void rp5c01SetHourMode(uint8_t mode) __z88dk_fastcall;
#define HR_MD_24 1
#define HR_MD_12 0

#define rp5c01SetByte(index, data) _rp5c01SetByte((((uint16_t)(index)) << 8) + (data))

extern void rp5c01SetMode(uint8_t mode) __z88dk_fastcall;
#define MD_TIME 8
#define MD_ALRM 4

typedef struct {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t day;
  uint8_t month;
  uint8_t year;
} rtcDateTime;

extern void rp5c01GetTime(rtcDateTime *) __z88dk_fastcall;
extern void rp5c01SetTime(rtcDateTime *) __z88dk_fastcall;

#endif
