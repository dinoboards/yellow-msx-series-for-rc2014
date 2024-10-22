#include "system_vars.h"

// F417: switch indicating if hooked up printer is an MSX printer or not
// values: 0: MSX-Printer, 1: no MSX-Printer
// if the printer is no MSX-Printer, non-ASCII (>=128) characters are replaced
// by spaces before sending them to the printer (ini: 0)
// RC2014 Extension - bit 7 indicate printer buffer has content
uint8_t NTMSXP_ADDR NTMSXP;

// FCA4-FCA5: parameter used at tap input, given a value during
// reading of a headerblock from tape
uint8_t LOWLIM_ADDR LOWLIM;
uint8_t WINWID_ADDR WINWID;

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

// F3AF: # of positions on a line in SCREEN 1 (ini:29)
uint8_t LINL32_ADDR LINL32;

// FFE7-FFF6: storage of VDP 8-23
uint8_t RG8SAV_ADDR RG8SAV[23 - 8];

// keyboard buffer; each char entered via the keyboard ends up here
char KEYBUF_ADDR KEYBUF[240];

// F672-F673: upper limit of memory area reserved for strings, contains the upper address
// that is allowed to be used
uint16_t MEMSIZ_ADDR MEMSIZ;

// F674-F675: top of stack; also first byte below string area
uint16_t STKTOP_ADDR STKTOP;

// FC48-FC49: lowest address of the RAM memory; initialized at startup
// and not changed normally
uint16_t BOTTOM_ADDR BOTTOM;

// FC4A-FC4B: highest address of the RAM memory that is not reserved by
// the OS; string area, filebuffers and stack are below this address
// initialized at startup and not changed normally
uint16_t HIMEM_ADDR HIMEM;

// F3F6: VDP-interupt counter that counts from 3 to 0, when it reaches zero, the
// keyboard matrix is scanned, and the counters is reset at 3
uint8_t SCNCNT_ADDR SCNCNT;

uint8_t SCRMOD_ADDR SCRMOD;

uint8_t HOKVLD_ADDR HOKVLD;

uint8_t EXTBIO_ADDR EXTBIO[5];

// F3DF-D3E6: storage for the last written value towards VDP registers 0 till 7
// this is needed because these registers are write only
uint8_t RG0SAV_ADDR RGS0TO7AV[8];
uint8_t RG0SAV_ADDR RG0SAV;
uint8_t RG1SAV_ADDR RG1SAV;
uint8_t RG2SAV_ADDR RG2SAV;
uint8_t RG3SAV_ADDR RG3SAV;
uint8_t RG4SAV_ADDR RG4SAV;
uint8_t RG5SAV_ADDR RG5SAV;
uint8_t RG6SAV_ADDR RG6SAV;
uint8_t RG7SAV_ADDR RG7SAV;

// FFE7-FFF6: storage of VDP 8-23
uint8_t RG8SAV_ADDR RG8SAV[23 - 8];

// FFFA-FFFC: storage of VDP 25-27
uint8_t RG25SAV_ADDR RG25SAV[27 - 25];
