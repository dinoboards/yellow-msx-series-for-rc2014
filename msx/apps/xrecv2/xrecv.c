#include "crt_override.h"
#include "fossil.h"
#include "msx.h"
#include "xmodem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VDP_FREQUENCY 50

#define LF    ('J' - 0x40)
#define CR    ('M' - 0x40)
#define SOH   ('A' - 0x40)
#define EOT   ('D' - 0x40)
#define ACK   ('F' - 0x40)
#define NAK   ('U' - 0x40)
#define CAN   ('X' - 0x40)
#define A_SUB ('Z' - 0x40)

#define TIMEDOUT 0xFF
#define ERR1     0xFE
#define ERR2     0xFD
#define ERR3     0xFC
#define ERR4     0xFB
#define ERR5     0xFA
#define OK       0x00

typedef struct {
  uint8_t *pHead;
  uint8_t *pTail;
  uint8_t  data[];
} rs232Buff;

uint8_t  __at 0xFB03 RS_TMP;
uint8_t  __at 0xFB1A RS_ERRORS;
uint8_t  __at 0xFB17 RS_DATCNT;
uint8_t  __at 0xFB1C RS_ESTBLS; // RTSON:		EQU	$		; Bit boolean. (RS-232C)
uint8_t  __at 0xFB1B RS_FLAGS;  // RS-232C bit flags
uint8_t *__at 0xFB18 RS_BUFEND;

rs232Buff *__at 0xFB04 RS_FCB;

const char *mydata = "this is data\r\n";

int _main(char **argv, int argc) {

  FILE *fptr;

  if (argc != 1) {
    printf("Usage:\r\nxrecv <filename>\r\n");
    exit(1);
  }

  const char *pFileName = argv[0];

  printf("%d, %s\r\n", argc, argv[0]);

  fptr = fopen(pFileName, "wb");

  // fwrite(mydata, strlen(mydata), 1, fptr);

  // return 0;

  if (!fossil_link()) {
    printf("Fossil driver not found\r\n");
    exit(1);
  }

  fossil_set_baud(9, 9);
  fossil_set_protocol(7); // 8N1
  fossil_init();

  printf("Send data using the xmodem protocol from your terminal emulator now...\n");

  XMODEM_SIGNAL sig = READ_FIRST_HEADER;
  while (sig = xmodem_receive(sig)) {
    if (sig & SAVE_PACKET) {
      fwrite(xmodemState.packetBuffer + 3, xmodemState.currentPacketSize, 1, fptr);
      printf(".");
    }
  }

  printf("finish reason sig: %d\r\n", xmodemState.finish_reason);

  fclose(fptr);

  // int st = xmodem_receive();
  // if (st < 0) {
  //   printf("Xmodem receive error: status: %d\n", st);
  // } else {
  //   printf("Xmodem successfully received %d bytes\n", st);
  // }

  fossil_deinit();
  return 0;
}
