#ifndef __SERIAL
#define __SERIAL

#include "fossil.h"

#define VDP_FREQUENCY 50

extern bool wait_for_byte(uint16_t period) __z88dk_fastcall;
extern void fossil_flush_input(uint16_t period) __z88dk_fastcall;

#ifdef DIAGNOSTIC_FILE_CAPTURE
extern void          serial_out(uint8_t ch) __z88dk_fastcall;
extern void          serial_flush_input(uint16_t period) __z88dk_fastcall;
extern unsigned char serial_in(void);
extern void          serial_in_block_start(const unsigned char *);
extern void          serial_in_block_end(const unsigned char *);
extern void          serial_diagnostic_message(const unsigned char *);

#else
#define serial_out         fossil_rs_out
#define serial_flush_input fossil_flush_input
#define serial_in          fossil_rs_in
#define serial_in_block_start(a)
#define serial_in_block_end(a)
#define serial_diagnostic_message(a)

#endif

#endif
