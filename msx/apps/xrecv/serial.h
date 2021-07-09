#ifndef __SERIAL
#define __SERIAL

#include "fossil.h"

#define VDP_FREQUENCY 50

extern bool waitForByte(uint8_t period) __z88dk_fastcall;

extern int _inbyte(uint8_t timeout) __z88dk_fastcall;

#define _outbyte(c) fossil_rs_out(c)

#endif
