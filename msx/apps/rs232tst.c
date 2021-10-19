
#include <extbio.h>
#include <msxdos.h>
#include <stdio.h>
#include <system_vars.h>

rs232_extbio_info info_table;
rs232_init_params init_params = {'8', 'N', '1', 'N', 'H', 'N', 'N', 'N', 19200, 19200, 0};

typedef struct {
  uint8_t *pHead;
  uint8_t *pTail;
  uint8_t  data[];
} rs232Buffer;

#define buffer_size 64
rs232Buffer __at 0xC000 buffer; // needs to be in page 3
uint8_t     __at 0xFB03 RS_TMP;
uint8_t     __at 0xFB1A RS_ERRORS;
uint8_t     __at 0xFB17 RS_DATCNT;
uint8_t     __at 0xFB1C RS_ESTBLS; // RTSON:		EQU	$		; Bit boolean. (RS-232C)
uint8_t     __at 0xFB1B RS_FLAGS;  // RS-232C bit flags
uint8_t *   __at 0xFB18 RS_BUFEND;

int main(const int argc, const unsigned char **argv) {
  (void)argc;

  const uint16_t i = atoi(argv[0]);
  printf("Configuring for baud rate: %d\r\n", i);

  init_params.receive_baud = init_params.send_baud = i;

  extbio_get_dev_info_table(8, &info_table);

  printf("Slot ID %02X, Address %p\r\n", info_table.slot_id, info_table.jump_table);

  rs232_link(&info_table);

  printf("Linked\r\n");

  const uint8_t r = rs232_init(&init_params);
  printf("init result %d\r\n", r);

  if (!r)
    goto exitApp;

  printf("buffersize: %d at %p\r\n", buffer_size, &buffer);
  rs232_open(RS232_RAW_MODE, buffer_size, (uint8_t *)&buffer);

  printf("BUFFER END: %p\r\n", RS_BUFEND);

  printf("1: %d\r\n", JIFFY);
  rs232_sndchr('A');
  printf("2: %d\r\n", JIFFY);
  rs232_sndchr('b');
  printf("2: %d\r\n", JIFFY);

  printf("\r\n");

  // while (1) {
  //   uint16_t timeout = 32000;

  //   int16_t count;

  //   if (msxbiosBreakX())
  //     goto exitApp;

  //   count = rs232_loc();

  //   while (count == 0 && timeout != 0) {
  //     if (timeout % 2048 == 0) {
  //       printf(".");
  //     }

  //     if (msxbiosBreakX())
  //       goto exitApp;

  //     count = rs232_loc();

  //     if (count != 0)
  //       break;

  //     timeout--;
  //   }

  //   if (timeout == 0) {

  //     printf("1: %d", JIFFY);
  //     rs232_sndchr('A');
  //     printf("2: %d", JIFFY);
  //     rs232_sndchr('b');
  //     printf("2: %d", JIFFY);

  //     printf("\r\n");
  //   }

  //   while (count > 0) {
  //     // printf(">> H: %p, T: %p, CNT: %d,\r\n", buffer.pHead, buffer.pTail, count);
  //     uint8_t ch = rs232_getchr();
  //     printf("%c", ch);
  //     // printf(">> H: %p, T: %p, CNT: %d, ch: %c\r\n", buffer.pHead, buffer.pTail, count, ch);

  //     count = rs232_loc();
  //   }
  // }

exitApp:
  printf("RS_FLAGS: %02X\r\nClosing...\r\n", RS_FLAGS);
  rs232_close();
  printf("RS_FLAGS: %02X\r\nClosed\r\n", RS_FLAGS);
  return 0;
}
