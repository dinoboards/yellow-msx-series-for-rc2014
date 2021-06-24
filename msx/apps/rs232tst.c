
#include "extbio.h"
#include "msxdos.h"
#include <stdio.h>

extbio_info info_table[4];
rs232_init_params init_params = {
  '8', 'N', '1', 'N', 'H', 'N', 'N', 'N', 19200, 19200, 0
};

typedef struct {
  uint8_t* pHead;
  uint8_t* pTail;
  uint8_t data[];
} rs232Buffer;

#define buffer_size 64
rs232Buffer __at 0xC000 buffer;     // needs to be in page 3
uint8_t __at 0xFB03 RSTMP;
uint8_t __at 0xFB1A RS_ERRORS;
uint8_t __at 0x0FB17 DATCNT;
uint8_t __at 0xFB1C ESTBLS;     // RTSON:		EQU	$		; Bit boolean. (RS-232C)
uint8_t __at 0xFB1B FLAGS;      // RS-232C flags

void main2();

void main() {
  extbio_get_dev_info_table(8, info_table);

  printf("Slot ID %02X, Address %p\r\n", info_table[0].slot_id, info_table[0].jump_table);

  rs232_link(&info_table[0]);

  rs232_init(&init_params);

  printf("buffersize: %d at %p\r\n", buffer_size, &buffer);
  rs232_open(RS232_RAW_MODE, buffer_size, (uint8_t*)&buffer);

  while(1) {
    uint16_t timeout = 32000;

    int16_t count;

    if (msxbiosBreakX())
      goto exitApp;


    count = rs232_loc();

    while(count == 0 && timeout != 0) {
      if (timeout % 2048 == 0) {
        printf(".");
      }

      if (msxbiosBreakX())
        goto exitApp;

      count = rs232_loc();

      if(count != 0)
        break;

      timeout--;
    }

    if (timeout == 0) {
      printf("\r\n");
    }

    while(count > 0) {
      printf(">> H: %p, T: %p, CNT: %d,\r\n", buffer.pHead, buffer.pTail, count);
      uint8_t ch = rs232_getchr();
      printf(">> H: %p, T: %p, CNT: %d, ch: %c\r\n", buffer.pHead, buffer.pTail, count, ch);

      count = rs232_loc();
    }
  }

exitApp:
  printf("FLAGS: %02X\r\nClosing...\r\n", FLAGS);
  rs232_close();
  printf("FLAGS: %02X\r\nClosed\r\n", FLAGS);
}
