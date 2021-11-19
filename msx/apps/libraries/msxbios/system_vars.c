#include "system_vars.h"

// FC9E-FC9F: software clock, updated at each VDP interrupt
uint16_t JIFFY_ADDR JIFFY;

// FBE5-FBEF: current state of the keyboard matrix
uint8_t NEWKEY_ADDR NEWKEY[10];

// F3F8-F3F9: first free space in the inputbuffer of the keyboard
// everytime a key is added to the inputbuffer, this address is incremented,
// when it equals to GETPNT, the buffer is full
// the buffer is located at KEYBUF
char *PUTPNT_ADDR PUTPNT;

// F3FA-F3FB: address in inputbuffer of first character that is not yet read
// everytime a key is read from the buffer it is incremented
// the buffer is located at KEYBUF
char *GETPNT_ADDR GETPNT;

// F3B3-F3B4: BASE(0): name table address for SCREEN 0 (ini:$0000)
// used to initialize NAMBAS when SCREEN 0 is activated
uint16_t TXTNAM_ADDR TXTNAM;

// F3DC: line where the cursor is located
// starts to count at 1 for the topmost line
uint8_t CSRY_ADDR CSRY;

// F3DD: column where the cursor is located
// starts to count at 1 for the leftmost column
uint8_t CSRX_ADDR CSRX;

// F3B0: # of actually used positions in the current screenmodus (ini:39)
uint8_t LINLEN_ADDR LINLEN;

// FCA9: show cursor; 0 = no, 1 = yes
// can be changed with escape sequences x5 and y5
uint8_t CSRSW_ADDR CSRSW;

// F3AE: # of positions on a line in SCREEN 0 (ini:39)
uint8_t LINL40_ADDR LINL40;

// FFE7-FFF6: storage of VDP 8-23
uint8_t RG8SAV_ADDR RG8SAV[23 - 8];
