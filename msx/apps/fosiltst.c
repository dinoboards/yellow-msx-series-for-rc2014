
#include "extbio.h"
#include "msxdos.h"
#include <stdio.h>
#include "xstdio.h"

typedef struct {
  uint8_t JP; // Usually 0xC9
  void *  function;
} jump_instruction;
typedef struct {
  jump_instruction getversion;
  jump_instruction init;
  jump_instruction deinit;
  jump_instruction setbaud;
  jump_instruction protocol;
  jump_instruction channel;
  jump_instruction rs_in;
  jump_instruction rs_out;
  jump_instruction rs_in_stat;
  jump_instruction rs_out_stat;
  jump_instruction dtr;
  jump_instruction rts;
  jump_instruction carrier;
  jump_instruction chars_in_buf;
  jump_instruction size_of_buf;
  jump_instruction flushbuf;
  jump_instruction fastint;
  jump_instruction hook38stat;
  jump_instruction chput_hook;
  jump_instruction keyb_hook;
  jump_instruction get_info;

} fossil_jump_table;

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
fossil_jump_table* __at 0xF400 FSTABL;

extern void fossil_link(void* jumpTable) __z88dk_fastcall;
extern uint16_t fossil_get_version();
extern void fossil_init();
extern void fossil_rs_out(char c) __z88dk_fastcall;

uint16_t __at 0xFD09 sltwork[64];

void main() {

  void *p = extbio_fossil_install();

  xprintf("Address %p\r\n", p);

  fossil_link(p);

  uint16_t version = fossil_get_version();

  xprintf("Version %04X\r\n", version);

  fossil_init();

  fossil_rs_out('A');
}
