#ifndef __APPMSX
#define __APPMSX

#include <stdlib.h>

#define JIFFY_ADDR  __at 0xFC9E
#define NEWKEY_ADDR __at 0xFBE5
#define PUTPNT_ADDR __at 0xF3F8
#define GETPNT_ADDR __at 0xF3FA
#define TXTNAM_ADDR __at 0xF3B3

// FC9E-FC9F: software clock, updated at each VDP interrupt
extern uint16_t JIFFY_ADDR JIFFY;

// FBE5-FBEF: current state of the keyboard matrix
extern uint8_t NEWKEY_ADDR NEWKEY[10];

// F3F8-F3F9: first free space in the inputbuffer of the keyboard
// everytime a key is added to the inputbuffer, this address is incremented,
// when it equals to GETPNT, the buffer is full
// the buffer is located at KEYBUF
extern char *PUTPNT_ADDR PUTPNT;

// F3FA-F3FB: address in inputbuffer of first character that is not yet read
// everytime a key is read from the buffer it is incremented
// the buffer is located at KEYBUF
extern char *GETPNT_ADDR GETPNT;

// F3B3-F3B4: BASE(0): name table address for SCREEN 0 (ini:$0000)
// used to initialize NAMBAS when SCREEN 0 is activated
extern uint16_t TXTNAM_ADDR TXTNAM;

#define msxJiffy  JIFFY
#define msxNewKey NEWKEY
#define msxPutPtr PUTPNT
#define msxGetPtr GETPNT

#define VDP_FREQUENCY 50

#endif
