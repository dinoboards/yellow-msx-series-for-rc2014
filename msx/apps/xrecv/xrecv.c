#include "fossil.h"
#include <stdio.h>
#include <stdlib.h>
#include "msx.h"

#define VDP_FREQUENCY 50

#define LF  ('J'-0x40)
#define CR  ('M'-0x40)
#define SOH ('A'-0x40)
#define EOT ('D'-0x40)
#define ACK ('F'-0x40)
#define NAK ('U'-0x40)
#define CAN ('X'-0x40)
#define A_SUB ('Z'-0x40)

#define TIMEDOUT 0xFF
#define ERR1 0xFE
#define ERR2 0xFD
#define ERR3 0xFC
#define ERR4 0xFB
#define ERR5 0xFA
#define OK 0x00

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


bool waitForByte() {
  const int16_t timeout = JIFFY + (VDP_FREQUENCY*2);

  while(JIFFY < timeout && !fossil_rs_in_stat())
    ;

  return fossil_rs_in_stat();
}

int sleep(int seconds) __z88dk_fastcall {
  const int16_t timeout = JIFFY + (VDP_FREQUENCY*seconds);

  while(JIFFY < timeout)
  ;
  return 0;
}

uint8_t packetNumber;
uint8_t buffer[128];


uint8_t read128Packet() {
  char ch;
  uint8_t sum = 0;

  if (!waitForByte())
    return TIMEDOUT;

  ch = fossil_rs_in();
  if (ch != packetNumber)
    return ERR1;

  if (!waitForByte())
    return TIMEDOUT;

  ch = fossil_rs_in();
  if (ch != (255-packetNumber))
    return ERR2;

  uint8_t *p = buffer;
  for(uint8_t i = 0; i < 128; i++) {
    if (!waitForByte())
      return TIMEDOUT;

    sum += (*p++ = fossil_rs_in());
  }

  if (!waitForByte())
    return TIMEDOUT;

  // checksum
  ch = fossil_rs_in();
  printf("Received packet??? %02X -> %02X\r\n", ch, sum);
  return OK;
}


uint8_t readPacketHeader() {
  const uint8_t ch = fossil_rs_in();

  switch(ch) {
    case EOT:
      return EOT;

    case CAN:
      return CAN;

    case SOH:
      return read128Packet();

    default:
      return ERR3;
  }
}

void firstPacket() {
  packetNumber = 1;

  for (uint8_t tries = 0; tries < 4; tries++) {
    fossil_rs_out(NAK);

    if(waitForByte())
      break;

    printf(".");
    sleep(4);
  }

  if (!waitForByte()) {
    printf("\r\nTimed out.\r\n");
    fossil_deinit();
    exit(1);
  }

  uint8_t r = readPacketHeader();
  printf("Result %02X\r\n", r);
}

void clearBuffers() {
  bool stat = fossil_rs_in_stat();

  while(stat) {
    const char ch = fossil_rs_in();
    printf("i.%02X ");

    stat = fossil_rs_in_stat();
  }
}

void main() {
  if (!fossil_link()) {
    printf("Fossil driver not found\r\n");
    exit(1);
  }

  fossil_set_baud(9, 9);
  fossil_set_protocol(7); // 8N1
  fossil_init();

  clearBuffers();

  firstPacket();
}

// // XYModemGet(0x50, 1);

// // Get filename and recreate the file

// packetNumber = 1;

// while(stillRetrying...) {
//   send(NAK);

//   timerStart = now
//   while(now - timerStart > 2 seconds && !fossil_rs_in_stat()) {
//     ;
//   }

//   if(fossil_rs_in_stat()) {
//     break - ready byte
//   }
// }

// if(!fossil_rs_in_stat) {
//   timed out
// }

// readFirstPacket
// }




// void xmodem_receive(const char* pFileName) {
//   (void)pFileName;

// }

// void readFirstPacket() {
//   const ch = fossil_rs_in();

//   switch(ch) {
//     case EOT:
//       finish

//     case CAN:
//       cancel

//     case SOH
//       readPacketBody

//     default:
//       error???
//   }
// }

// uint8_t buffer[132];

// void readPacketBody() {
//   char ch;
//   uint8_t *p = buffer;

//   for(uint8_t i = 0; i < 131; i++) {
//     const r = getByte(&ch) // with timeout?

//     if(!r)
//       timedout

//     *p++ = ch;
//   }

//   const x = sumOf(buffer)

// }
