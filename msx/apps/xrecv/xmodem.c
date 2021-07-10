/*
 * Copyright 2001-2010 Georges Menie (www.menie.org)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* this code needs standard functions memcpy() and memset()
   and input/output functions _inbyte() and _outbyte().

   the prototypes of the input/output functions are:
     int _inbyte(unsigned short timeout); // msec timeout
     void _outbyte(int c);

 */

#include "xmodem.h"
#include "crc16.h"
#include "serial.h"
#include <stdio.h>

#define SOH   0x01
#define STX   0x02
#define EOT   0x04
#define ACK   0x06
#define NAK   0x15
#define CAN   0x18
#define CTRLZ 0x1A

#define DLY_1S     VDP_FREQUENCY
#define MAXRETRANS 25
#define TRANSMIT_XMODEM_1K

static int check(int crc, const unsigned char *buf, int sz) {
  if (crc) {
    const unsigned short crc = crc16_ccitt(buf, sz);
    const unsigned short tcrc = (buf[sz] << 8) + buf[sz + 1];
    return (crc == tcrc);
  }

  int           i;
  unsigned char cks = 0;
  for (i = 0; i < sz; ++i) {
    cks += *buf++;
  }
  return (cks == buf[sz]);
}

static void flushinput(void) {
  while (_inbyte(((DLY_1S)*3) >> 1) >= 0)
    ;
}

struct xmodemState xmodemState;

int           crc = 0;
unsigned char trychar = 'C';
unsigned char packetno = 1;
int           i, c, len = 0;
int           retry, retrans = MAXRETRANS;

bool readPacket() {
  unsigned char *p = xmodemState.packetBuffer;

  *p++ = c;
  for (i = 0; i < (xmodemState.currentPacketSize + (crc) + 3); ++i) {
    if (!waitForByte(DLY_1S))
      return false;
    *p++ = fossil_rs_in();
  }

  return true;
}

XMODEM_SIGNAL readHeader() {
  if (trychar)
    _outbyte(trychar);

  if (waitForByte(DLY_1S)) {
    switch (fossil_rs_in()) {
    case SOH:
      xmodemState.currentPacketSize = 128;
      return READ_128;

    case STX:
      xmodemState.currentPacketSize = 1024;
      return READ_1024;

    case EOT:
      flushinput();
      _outbyte(ACK);
      return END_OF_STREAM;

    case CAN:
      if ((c = _inbyte(DLY_1S)) == CAN) {
        flushinput();
        _outbyte(ACK);
        return UPSTREAM_CANCELLED;
      }
      break;
    }
  }

  if (trychar == 'C') {
    trychar = NAK;
    return TRY_AGAIN;
  }

  flushinput();
  _outbyte(CAN);
  _outbyte(CAN);
  _outbyte(CAN);
  return STREAM_ERROR;
}

XMODEM_SIGNAL startRecv() {
  if (trychar == 'C')
    crc = 1;
  trychar = 0;

  if (!readPacket())
    return PACKET_REJECT;

  if (xmodemState.packetBuffer[1] == (unsigned char)(~xmodemState.packetBuffer[2]) && (xmodemState.packetBuffer[1] == packetno) && check(crc, &xmodemState.packetBuffer[3], xmodemState.currentPacketSize))
    return SAVE_PACKET;

  if (--retrans <= 0) {
    flushinput();
    _outbyte(CAN);
    _outbyte(CAN);
    _outbyte(CAN);
    return TOO_MANY_ERRORS;
  }

  return PACKET_REJECT;
}

XMODEM_SIGNAL xmodemReceive(XMODEM_SIGNAL signal) __z88dk_fastcall {

  switch(signal) {
    case READ_HEADER:
      if (retry >= 16)
        return TOO_MANY_ERRORS; // send the three CAN

      return readHeader();

    case READ_128:
    case READ_1024:
      return startRecv();

    case NEXT_PACKET:
      retry = 0;
      return READ_HEADER;

    case PACKET_REJECT:
    case TRY_AGAIN:
      flushinput();
      _outbyte(NAK);
      retry++;
      return READ_HEADER;

    case SAVE_PACKET:
      ++packetno;
      retrans = MAXRETRANS + 1;
      _outbyte(ACK);
      return NEXT_PACKET;

    // case END_OF_STREAM:
    // case UPSTREAM_CANCELLED:
    // case STREAM_ERROR:
    // case TOO_MANY_ERRORS:
    //   return FINISHED;

    default:
      return FINISHED;
  }
}

// int xmodemTransmit()
// {
//   const int srcsz = 10;

// 	unsigned char xmodemState.packetBuffer[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
// 	int xmodemState.currentPacketSize, crc = -1;
// 	unsigned char packetno = 1;
// 	int i, c, len = 0;
// 	int retry;

// 	for(;;) {
// 		for( retry = 0; retry < 16; ++retry) {
// 			if ((c = _inbyte((DLY_1S)<<1)) >= 0) {
// 				switch (c) {
// 				case 'C':
// 					crc = 1;
// 					goto start_trans;
// 				case NAK:
// 					crc = 0;
// 					goto start_trans;
// 				case CAN:
// 					if ((c = _inbyte(DLY_1S)) == CAN) {
// 						_outbyte(ACK);
// 						flushinput();
// 						return -1; /* canceled by remote */
// 					}
// 					break;
// 				default:
// 					break;
// 				}
// 			}
// 		}
// 		_outbyte(CAN);
// 		_outbyte(CAN);
// 		_outbyte(CAN);
// 		flushinput();
// 		return -2; /* no sync */

// 		for(;;) {
// 		start_trans:
// #ifdef TRANSMIT_XMODEM_1K
// 			xmodemState.packetBuffer[0] = STX; xmodemState.currentPacketSize = 1024;
// #else
// 			xmodemState.packetBuffer[0] = SOH; xmodemState.currentPacketSize = 128;
// #endif
// 			xmodemState.packetBuffer[1] = packetno;
// 			xmodemState.packetBuffer[2] = ~packetno;
// 			c = srcsz - len;
// 			if (c > xmodemState.currentPacketSize) c = xmodemState.currentPacketSize;
// 			if (c > 0) {
// 				// memset (&xmodemState.packetBuffer[3], 0, xmodemState.currentPacketSize);
// 				// memcpy (&xmodemState.packetBuffer[3], &src[len], c);
// 				if (c < xmodemState.currentPacketSize) xmodemState.packetBuffer[3+c] = CTRLZ;
// 				if (crc) {
// 					unsigned short ccrc = crc16_ccitt(&xmodemState.packetBuffer[3], xmodemState.currentPacketSize);
// 					xmodemState.packetBuffer[xmodemState.currentPacketSize+3] = (ccrc>>8) & 0xFF;
// 					xmodemState.packetBuffer[xmodemState.currentPacketSize+4] = ccrc & 0xFF;
// 				}
// 				else {
// 					unsigned char ccks = 0;
// 					for (i = 3; i < xmodemState.currentPacketSize+3; ++i) {
// 						ccks += xmodemState.packetBuffer[i];
// 					}
// 					xmodemState.packetBuffer[xmodemState.currentPacketSize+3] = ccks;
// 				}
// 				for (retry = 0; retry < MAXRETRANS; ++retry) {
// 					for (i = 0; i < xmodemState.currentPacketSize+4+(crc?1:0); ++i) {
// 						_outbyte(xmodemState.packetBuffer[i]);
// 					}
// 					if ((c = _inbyte(DLY_1S)) >= 0 ) {
// 						switch (c) {
// 						case ACK:
// 							++packetno;
// 							len += xmodemState.currentPacketSize;
// 							goto start_trans;
// 						case CAN:
// 							if ((c = _inbyte(DLY_1S)) == CAN) {
// 								_outbyte(ACK);
// 								flushinput();
// 								return -1; /* canceled by remote */
// 							}
// 							break;
// 						case NAK:
// 						default:
// 							break;
// 						}
// 					}
// 				}
// 				_outbyte(CAN);
// 				_outbyte(CAN);
// 				_outbyte(CAN);
// 				flushinput();
// 				return -4; /* xmit error */
// 			}
// 			else {
// 				for (retry = 0; retry < 10; ++retry) {
// 					_outbyte(EOT);
// 					if ((c = _inbyte((DLY_1S)<<1)) == ACK) break;
// 				}
// 				flushinput();
// 				return (c == ACK)?len:-5;
// 			}
// 		}
// 	}
// }
