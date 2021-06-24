
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

    if (msxbiosBreakX()) {
      exit(0);
    }

    count = rs232_loc();

    while(count == 0 && timeout != 0) {
      if (timeout % 2048 == 0) {
        printf(".%d.%d.%p.", DATCNT, count, buffer.pHead);
      }

      if (msxbiosBreakX()) {
        exit(0);
      }

      count = rs232_loc();

      if(count != 0)
        break;

      timeout--;
    }

    if (timeout == 0) {
      printf("t");
      rs232_sndchr('T');
    }

    while(count > 0) {
      uint8_t ch = rs232_getchr();

      printf("%c", ch);

      count = rs232_loc();
    }
  }

  // rs232_close();

}
