#ifndef __APPMSX
#define __APPMSX

#include <stdlib.h>

#define JIFFY_ADDR  __at 0xFC9E
#define NEWKEY_ADDR __at 0xFBE5
#define PUTPNT_ADDR __at 0xF3F8
#define GETPNT_ADDR __at 0xF3FA
#define TXTNAM_ADDR __at 0xF3B3
#define CSRY_ADDR   __at 0xF3DC
#define CSRX_ADDR   __at 0xF3DD
#define LINLEN_ADDR __at 0xF3B0
#define CSRSW_ADDR  __at 0xFCA9
#define LINL40_ADDR __at 0xF3AE
#define RG8SAV_ADDR __at 0xFFE7

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

// F3DC: line where the cursor is located
// starts to count at 1 for the topmost line
extern uint8_t CSRY_ADDR CSRY;

// F3DD: column where the cursor is located
// starts to count at 1 for the leftmost column
extern uint8_t CSRX_ADDR CSRX;

// F3B0: # of actually used positions in the current screenmodus (ini:39)
extern uint8_t LINLEN_ADDR LINLEN;

// FCA9: show cursor; 0 = no, 1 = yes
// can be changed with escape sequences x5 and y5
extern uint8_t CSRSW_ADDR CSRSW;

// F3AE: # of positions on a line in SCREEN 0 (ini:39)
extern uint8_t LINL40_ADDR LINL40;

#define msxJiffy  JIFFY
#define msxNewKey NEWKEY
#define msxPutPtr PUTPNT
#define msxGetPtr GETPNT

#define VDP_FREQUENCY 50

// FFE7-FFF6: storage of VDP 8-23
extern uint8_t RG8SAV_ADDR RG8SAV[23 - 8];

#define GET_VDP_REGISTER(a) ((a >= 8 && a <= 23) ? RG8SAV[a] : 0)

#define GET_VDP_FREQUENCY() (GET_VDP_REGISTER(9) & 2 ? 60 : 50)

#endif
