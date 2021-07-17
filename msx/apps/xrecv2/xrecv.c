#include "arguments.h"
#include "crt_override.h"
#include "fossil.h"
#include "msx.h"
#include "xmodem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * TODO:
 * [x] Clean up copy on start
 * [x] Update usage help copy
 * [x] Print actual selected baud rate
 * [ ] Monitor for CTRL+BREAK and perhaps CTR+C to abort
 * [ ] Dont create/open file is any errors - maybe create a tmp and then rename?
 * [ ] Minimise code size if possible
 * [ ] Improved error handling reporting
 * [ ] Show file size download progress
 * [ ] Auto activate fossil driver

*/

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

int _main(const char **argv, const int argc) {
  if (!fossil_link()) {
    printf("Fossil driver not found\r\n");
    exit(1);
  }

  process_cli_arguments(argv, argc);

  FILE *fptr;

  printf("Filename: %s\r\n", pFileName);
  printf("Requested Baud Rate: %s\r\n", fossil_rate_to_string(baud_rate));
  uint16_t actual_baud_rate = fossil_set_baud(baud_rate);
  if (actual_baud_rate != baud_rate)
    printf("Actual Baud Rate: %s\r\n", fossil_rate_to_string(actual_baud_rate));

  fptr = fopen(pFileName, "wb");

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
