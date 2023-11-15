#ifndef __XMODEM_SERIAL
#define __XMODEM_SERIAL

#include "fossil.h"

#define VDP_FREQUENCY 50

extern bool wait_for_byte(uint16_t period) __z88dk_fastcall;
extern void fossil_flush_input(uint16_t period) __z88dk_fastcall;

#endif
