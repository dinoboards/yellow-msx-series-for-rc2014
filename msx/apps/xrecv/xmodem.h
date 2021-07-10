#ifndef __XMODEM
#define __XMODEM

#include <stdlib.h>

typedef enum { FINISHED = 0, READ_HEADER = 1, READ_128, READ_1024, SAVE_PACKET, NEXT_PACKET, END_OF_STREAM, UPSTREAM_CANCELLED, STREAM_ERROR, TRY_AGAIN, PACKET_REJECT, TOO_MANY_ERRORS } XMODEM_SIGNAL;

extern XMODEM_SIGNAL xmodemReceive(XMODEM_SIGNAL signal) __z88dk_fastcall;

typedef struct xmodemState {
  unsigned char packetBuffer[1030];
  uint16_t  currentPacketSize;
};

extern struct xmodemState xmodemState;

#endif
