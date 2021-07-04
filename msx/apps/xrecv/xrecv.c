#include "fossil.h"
#include <stdio.h>

void main() {
  if (!fossil_link()) {
    printf("Fossil driver not found\r\n");
    exit(1);
  }

  fossil_set_baud(9, 9);
  fossil_set_protocol(7); // 8N1
  fossil_init();

  printf("TODO!");
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

// bool getByte(const char* p) __z88dk_fastcall {

// }

// uint8_t packetNumber;

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
