
#include "extbio.h"
#include "msxdos.h"
#include <stdio.h>

// typedef struct {
//   uint8_t JP; // Usually 0xC9
//   void *  function;
// // } jump_instruction;
// typedef struct {
//   uint8_t          infoBits; // MSX serial features (no TxReady INT, No Sync detect, No Timer INT, No CD, No RI)
//   uint8_t          version;  // version number
//   uint8_t          reserved; // reserved for future expansion
//   jump_instruction init;     // initialize RS232C port
//   jump_instruction open;     // open RS232C port
//   jump_instruction stat;     // ReaD STATus
//   jump_instruction getchr;   // reveive data
//   jump_instruction sndchr;   // send data
//   jump_instruction close;    // close RS232C port
//   jump_instruction eof;      // tell EOF code received
//   jump_instruction loc;      // reports number of characters in the
//                              // receiver buffer
//   jump_instruction lof;      // reports number of free space left in the
//                              // receiver buffer
//   jump_instruction backup;   // back up a character
//   jump_instruction sndbrk;   // send break character
//   jump_instruction dtr;      // turn on/off DTR line
//   jump_instruction setchn;   // set channel number

// } rs232_jump_table;

// typedef struct {
//   uint8_t RST_30H;
//   uint8_t slot;
//   void* addr;
//   uint8_t RET;
// } slot_jump_instruction;
// typedef struct {
//   slot_jump_instruction init;
// } rs232_slot_jumps;


char __at 0xF3F3  FSMARK[2];
void* __at 0xF400 FSTABL;

extern void fossil_link(void* jumpTable) __z88dk_fastcall;
extern uint16_t fossil_get_version();

void main() {

  printf("FSMARK %c%c\r\n", FSMARK[0], FSMARK[1]);
  printf("FSTABL %p\r\n", FSTABL);

  void *p = extbio_fossil_install();

  printf("Address %p\r\n", p);

  printf("FSMARK %c%c\r\n", FSMARK[0], FSMARK[1]);
  printf("FSTABL %p\r\n", FSTABL);

  fossil_link(p);

  uint16_t version = fossil_get_version();

  printf("Version %04X\r\n", version);
}
