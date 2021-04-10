#define __Z88DK_R2L_CALLING_CONVENTION
#include "msxdos.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "telnet/fossil_interface.h"

extern void    extbio_get_dev(uint8_t *table) __z88dk_fastcall;
extern void    getsystem();
extern uint8_t table[32];

const char *extendedBiosName(uint8_t id) {
  switch (id) {
  case 0:
    return "Broadcase";

  case 4:
    return "MSX-DOS Memory Mapper";

  case 8:
    return "MSX-Modem & RS-232C";

  case 10:
    return "MSX-AUDIO";

  case 16:
    return "MSX-JE";

  case 17:
    return "Kanji driver";

  case 34:
    return "MSX-UNAPI";

  case 77:
    return "MemMan";

  case 132:
    return "μ-driver by Yoshikazu Yamamoto";

  case 241:
    return "MultiMente";

  default:
    return "Unknown";
  }
}

uint8_t table[32];

typedef struct {
  uint8_t slot_id;
  void *  jump_table;
  uint8_t reserved;
} extbio_info;

extbio_info info_table[4];

typedef struct {
  char     character_length;    // Character length '5'-'8'
  char     parity;              // Parity 'E','O','I','N'
  char     stop_bits;           // Stop bits '1','2','3'
  char     xon_xoff_control;    // XON/XOFF controll 'X','N'
  char     handshake;           // CTR-RTS hand shake 'H','N'
  char     autoLineFeedSend;    // Auto LF for receive 'A','N'
  char     autoLineFeedReceive; // Auto LF for send 'A','N'
  char     si_so_control;       // SI/SO control 'S','N'
  uint16_t receive_baud;        // 50-19200
  uint16_t send_baud;           // 50-19200
  uint8_t  timeout_counter;     // Time out counter 0-255
} rs232_init_params;

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

rs232_init_params init_params = {
  '8', 'N', '1', 'N', 'H', 'N', 'N', 'N', 19200, 19200, 0
};

typedef struct {
  uint8_t reserved[9];
  uint16_t buffer[];
} rs232_fcb;

typedef enum { RS232_RAW_MODE = 2 } rs232_io_mode;

extern void extbio_get_dev_info_table(uint8_t device_id, extbio_info *info_table);
extern void rs232_link(extbio_info *p) __z88dk_fastcall;
extern void rs232_init(rs232_init_params*) __z88dk_fastcall;
extern void rs232_open(uint8_t mode, uint8_t buffer_size, rs232_fcb* fcb);
extern void rs232_close();
extern uint8_t rs232_sndchr(const char ch) __z88dk_fastcall;
extern uint16_t rs232_getchr();
extern uint16_t rs232_loc();


#define BUFFER_SIZE 32
uint8_t buffer[sizeof(rs232_fcb)+BUFFER_SIZE*2];

void main() {
  const bool extendedBiosReady = HOKVLD & 1;
  if (!extendedBiosReady) {
    printf("No bios extenstions installed\r\n");
    return;
  }

  const bool atLeastOneInstalled = EXTBIO[0] != RET_INSTRUCTION;
  if (!atLeastOneInstalled) {
    printf("No bios extensions installed\r\n");
    return;
  }

  printf("\r\nInstalled Extended Bios Systems:\r\n");
  printf("-----------------------------------\r\n");

  extbio_get_dev(table);

  for (int i = 0; i < 32 && table[i]; i++)
    printf("%d: %s (%02X)\r\n", i, extendedBiosName(table[i]), table[i]);

  printf("\r\n");

  /*** GET RS232 SERIAL SLOT NUMBERS ***/

  extbio_get_dev_info_table(8, info_table);

  printf("Slot ID %02X, Address %p\r\n", info_table[0].slot_id, info_table[0].jump_table);

  rs232_link(&info_table[0]);

  rs232_init(&init_params);

  rs232_open(RS232_RAW_MODE, BUFFER_SIZE, (rs232_fcb*)buffer);

  rs232_sndchr('A');

  uint16_t count = rs232_loc();
  printf("\r\nLOC: %d\r\n", count);

  uint8_t ch = rs232_getchr();

  printf("\r\nRead ch: %c\r\n", ch);

  rs232_close();

  // //lets try for a fossil implementation
  // fossile_jump_table jtable;
  // fossil_ex_link(&jtable);


  // printf("Fossil Extended System found? %02X, %p\r\n", jtable.slot_id, jtable.jump_table);
}
