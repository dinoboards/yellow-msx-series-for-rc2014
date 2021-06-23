
#include "extbio.h"
#include "msxdos.h"
#include <stdio.h>

extbio_info info_table[4];
rs232_init_params init_params = {
  '8', 'N', '1', 'N', 'H', 'N', 'N', 'N', 19200, 19200, 0
};

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

typedef struct {
  uint8_t* pHead;
  uint8_t pTail;
  uint8_t data[];
} rs232Buffer;

#define buffer_size 64
// uint8_t __at 0xC000 buffer[128+9];    // needs to be in page 3
rs232Buffer __at 0xC000 buffer; //[128+9];    // needs to be in page 3
uint8_t __at 0xFB03 RSTMP;
uint8_t __at 0xFB1A RS_ERRORS;
uint8_t __at 0x0FB17 DATCNT;
uint8_t __at 0xFB1C ESTBLS;     // RTSON:		EQU	$		; Bit boolean. (RS-232C)

extern void disableVdpInterrupts();
extern void enableVdpInterrupts();

char print_buffer[256];

void main() {

  // disableVdpInterrupts();

  extbio_get_dev_info_table(8, info_table);

  printf("Slot ID %02X, Address %p\r\n", info_table[0].slot_id, info_table[0].jump_table);

  rs232_link(&info_table[0]);

  rs232_init(&init_params);

  printf("buffersize: %d at %p\r\n", buffer_size, &buffer);
  rs232_open(RS232_RAW_MODE, buffer_size, (uint8_t*)&buffer);

  rs232_sndchr('A');

  while(1) {
    int16_t timeout = 16000;

    int16_t count;

    if (msxbiosBreakX()) {
      enableVdpInterrupts();
        exit(0);
    }

    count = rs232_loc();

    // printf("1 -- %ld\r\n", timeout);
    while(count == 0 && timeout-- > 0) {

      if (timeout % 2000 == 0) {
        printf(".%d-%d.", ESTBLS, DATCNT);
      }

      if (msxbiosBreakX()) {
        enableVdpInterrupts();
         exit(0);
      }

      count = rs232_loc();
    }

    if (count == 0) {
      printf("t");
      rs232_sndchr('T');
    }

    if (count > 0) {
      uint8_t index = 0;
      while(count > 0 && index < 250) {
        uint8_t ch = rs232_getchr();

        print_buffer[index++] = ch;
        print_buffer[index] = 0;

        printf("%c", ch);

        count = rs232_loc();
      }

      printf("\r\n--");
      printf(print_buffer);
      printf("--\r\n");
    }
  }

  // rs232_close();

}
