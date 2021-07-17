#include "crt_override.h"
#include "fossil.h"
#include "msx.h"
#include "xmodem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * TODO:
 * [ ] Clean up copy on start
 * [ ] Update usage help copy
 * [ ] Print actual selected baud rate
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

const char *mydata = "this is data\r\n";

uint16_t baud_rate = 9 * 256 + 9;

uint8_t arg_baud_rate(uint8_t i, char **argv) {
  const char *arg_switch = argv[i];
  if (strcmp(arg_switch, "-b") != 0 && strcmp(arg_switch, "--baud") != 0)
    return i;

  const char *arg_value = argv[i + 1];
  i += 1;

  printf("Baud rate of %s\r\n", arg_value);

  if (strcmp(arg_value, "75") == 0) {
    baud_rate = 0;
    return i;
  }

  if (strcmp(arg_value, "300") == 0) {
    baud_rate = 1 * 256 + 1;
    return i;
  }

  if (strcmp(arg_value, "600") == 0) {
    baud_rate = 2 * 256 + 2;
    return i;
  }

  if (strcmp(arg_value, "1200") == 0) {
    baud_rate = 3 * 256 + 3;
    return i;
  }

  if (strcmp(arg_value, "2400") == 0) {
    baud_rate = 4 * 256 + 4;
    return i;
  }

  if (strcmp(arg_value, "4800") == 0) {
    baud_rate = 5 * 256 + 5;
    printf("Selected 4800\r\n");
    return i;
  }

  if (strcmp(arg_value, "9600") == 0) {
    baud_rate = 6 * 256 + 6;
    return i;
  }

  if (strcmp(arg_value, "19200") == 0) {
    baud_rate = 7 * 256 + 7;
    return i;
  }

  if (strcmp(arg_value, "38400") == 0) {
    baud_rate = 8 * 256 + 8;
    return i;
  }

  if (strcmp(arg_value, "57600") == 0) {
    baud_rate = 9 * 256 + 9;
    return i;
  }

  if (strcmp(arg_value, "115200") == 0) {
    baud_rate = 11 * 256 + 11;
    return i;
  }

  printf("Invalid baud rate setting '%s'\r\n", argv[i + 1]);
  exit(1);

  return 100;
}

int _main(char **argv, int argc) {
  if (!fossil_link()) {
    printf("Fossil driver not found\r\n");
    exit(1);
  }

  FILE *fptr;

  if (argc < 1) {
    printf("Usage:\r\nxrecv <filename>\r\n");
    exit(1);
  }

  for (uint8_t i = 0; i < argc; i++) {
    i = arg_baud_rate(i, argv);
  }

  printf("Baud rate %04X\r\n", baud_rate);

  const char *pFileName = argv[0];

  printf("%d, %s\r\n", argc, argv[0]);

  fptr = fopen(pFileName, "wb");

  uint16_t actual_baud_rate = fossil_set_baud(baud_rate);

  printf("Actual Baud Rate: %04X\r\n", actual_baud_rate);

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