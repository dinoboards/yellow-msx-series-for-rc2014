#include "xmodem.h"
#include "crc16.h"
#include "serial.h"
#include <stdio.h>
#include <system_vars.h>

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

#define FOR_SIGNAL(mask) if (signal & (mask))

struct xmodemState xmodemState;
unsigned char      packetno = 1;
uint16_t           i;
uint16_t           retry, retrans = MAXRETRANS;

static int16_t       delay_point;
static XMODEM_SIGNAL delay_resume;

inline XMODEM_SIGNAL delay_start(uint16_t period, XMODEM_SIGNAL next_signal) {
  delay_point = ((int16_t)JIFFY) + (period);
  delay_resume = (next_signal);
  return DELAY_WAIT;
}

XMODEM_SIGNAL delay_reached() { return ((delay_point - ((int16_t)JIFFY) >= 0)) ? DELAY_WAIT : delay_resume; }

static bool check_crc() {
  const unsigned char *buf = &xmodemState.packetBuffer[3];
  const uint16_t       sz = xmodemState.currentPacketSize;
  uint16_t             counter;
  uint16_t             crc = 0;
  for (counter = 0; counter < sz; counter++)
    crc = (crc << 8) ^ crc16tab[((crc >> 8) ^ *buf++) & 0x00FF];

  const uint16_t tcrc = (*buf++ << 8) + *buf;
  return (crc == tcrc);
}

static bool check_sum() {
  const unsigned char *buf = &xmodemState.packetBuffer[3];
  const uint16_t       sz = xmodemState.currentPacketSize;

  uint8_t cks = 0;
  for (uint16_t i = 0; i < sz; ++i) {
    cks += *buf++;
  }
  return (cks == *buf);
}

static void flush_input(void) { serial_flush_input((DLY_1S * 3) >> 1); }

bool read_packet_crc() {
  unsigned char *p = xmodemState.packetBuffer;

  serial_in_block_start("\r\nReceiving Packet with CRC\r\n");

  p++;
  for (i = 0; i < (xmodemState.currentPacketSize + 1 + 3); ++i) {
    if (!wait_for_byte(DLY_1S)) {
      serial_in_block_end("\r\nTimed out waiting for a full CRC packet to arrive.\r\n");
      return false;
    }
    *p++ = serial_in();
  }

  serial_in_block_end(NULL);
  return true;
}

bool read_packet_sum() {
  unsigned char *p = xmodemState.packetBuffer;

  serial_in_block_start("\r\nReceiving Packet with CHKSUM\r\n");

  p++;
  for (i = 0; i < (xmodemState.currentPacketSize + 0 + 3); ++i) {
    if (!wait_for_byte(DLY_1S)) {
      serial_in_block_end("\r\nTimed out waiting for a full CHKSUM packet to arrive.\r\n");
      return false;
    }
    *p++ = serial_in();
  }

  serial_in_block_end(NULL);

  return true;
}

XMODEM_SIGNAL read_first_header() {
  packetno = 1;
  retry = retrans = MAXRETRANS;

  serial_out('C');

  if (wait_for_byte(DLY_1S * 10)) {
    switch (serial_in()) {
    case SOH:
      xmodemState.currentPacketSize = 128;
      return READ_CRC | READ_128;

    case STX:
      xmodemState.currentPacketSize = 1024;
      return READ_CRC | READ_1024;

    case EOT:
      serial_out(ACK);
      return END_OF_STREAM;

    case CAN:
      flush_input();
      serial_out(ACK);
      return UPSTREAM_CANCELLED;
    }
  }

  return delay_start(DLY_1S * 4, TRY_AGAIN);
}

XMODEM_SIGNAL read_header(const XMODEM_SIGNAL signal) __z88dk_fastcall {
  if (wait_for_byte(DLY_1S * 10)) {
    switch (serial_in()) {
    case SOH:
      xmodemState.currentPacketSize = 128;
      return signal | READ_128;

    case STX:
      xmodemState.currentPacketSize = 1024;
      return signal | READ_1024;

    case EOT:
      serial_out(ACK);
      return END_OF_STREAM;

    case CAN:
      flush_input();
      serial_out(ACK);
      return UPSTREAM_CANCELLED;
    }
  }

  return delay_start(DLY_1S * 4, signal | TRY_AGAIN);
}

XMODEM_SIGNAL start_receive_crc(const XMODEM_SIGNAL signal) __z88dk_fastcall {
  if (!read_packet_crc())
    return delay_start(DLY_1S * 4, signal | PACKET_TIMEOUT | STREAM_ERROR);

  serial_diagnostic_message("\r\nPacket Received via CRC\r\n");

  if (xmodemState.packetBuffer[1] == (unsigned char)(~xmodemState.packetBuffer[2]) && (xmodemState.packetBuffer[1] == packetno) && check_crc())
    return signal | SAVE_PACKET;

  serial_diagnostic_message("\r\nCRC failure\r\n");

  if (--retrans <= 0) {
    flush_input();
    serial_out(CAN);
    serial_out(CAN);
    serial_out(CAN);
    return TOO_MANY_ERRORS;
  }

  return delay_start(DLY_1S * 4, signal | PACKET_REJECT);
}

XMODEM_SIGNAL start_receive_checksum(const XMODEM_SIGNAL signal) __z88dk_fastcall {
  if (!read_packet_sum())
    return delay_start(DLY_1S * 4, signal | PACKET_TIMEOUT | STREAM_ERROR);

  serial_diagnostic_message("\r\nPacket Received via CHKSUM\r\n");

  if (xmodemState.packetBuffer[1] == (unsigned char)(~xmodemState.packetBuffer[2]) && (xmodemState.packetBuffer[1] == packetno) && check_sum())
    return signal | SAVE_PACKET;

  serial_diagnostic_message("\r\nCHKSUM failure\r\n");

  if (--retrans <= 0) {
    flush_input();
    serial_out(CAN);
    serial_out(CAN);
    serial_out(CAN);
    return TOO_MANY_ERRORS;
  }

  return delay_start(DLY_1S * 4, signal | PACKET_REJECT);
}

XMODEM_SIGNAL xmodem_tryagain(const XMODEM_SIGNAL signal) __z88dk_fastcall {
  flush_input();

  if (retry++ >= 5)
    return TOO_MANY_ERRORS;

  fputc_cons('.');
  serial_out(NAK);
  retry++;
  return signal | READ_HEADER;
}

XMODEM_SIGNAL xmodem_packet_save(const XMODEM_SIGNAL signal) __z88dk_fastcall {
  serial_out(ACK);
  ++packetno;
  retrans = MAXRETRANS + 1;
  retry = 0;
  return signal | READ_HEADER;
}

XMODEM_SIGNAL xmodem_too_many_errors() __z88dk_fastcall {
  flush_input();
  serial_out(CAN);
  serial_out(CAN);
  serial_out(CAN);
  xmodemState.finish_reason = TOO_MANY_ERRORS;
  return FINISHED;
}

XMODEM_SIGNAL xmodem_receive(const XMODEM_SIGNAL signal) __z88dk_fastcall {
  FOR_SIGNAL(DELAY_WAIT) { return delay_reached(); }

  FOR_SIGNAL(READ_FIRST_HEADER) { return read_first_header(); }

  FOR_SIGNAL(READ_HEADER) { return read_header(signal & ~READ_HEADER); }

  FOR_SIGNAL(READ_128 | READ_1024) {
    FOR_SIGNAL(READ_CRC) { return start_receive_crc(signal & ~(READ_128 | READ_1024)); }
    else {
      return start_receive_checksum(signal & ~(READ_128 | READ_1024));
    }
  }

  FOR_SIGNAL(PACKET_TIMEOUT | PACKET_REJECT | TRY_AGAIN) { return xmodem_tryagain(signal & ~(PACKET_TIMEOUT | PACKET_REJECT | TRY_AGAIN)); }

  FOR_SIGNAL(SAVE_PACKET) { return xmodem_packet_save(signal & ~SAVE_PACKET); }

  FOR_SIGNAL(TOO_MANY_ERRORS) { return xmodem_too_many_errors(); }

  xmodemState.finish_reason = signal;
  return FINISHED;
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
// 						serial_out(ACK);
// 						flush_input();
// 						return -1; /* canceled by remote */
// 					}
// 					break;
// 				default:
// 					break;
// 				}
// 			}
// 		}
// 		serial_out(CAN);
// 		serial_out(CAN);
// 		serial_out(CAN);
// 		flush_input();
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
// 						serial_out(xmodemState.packetBuffer[i]);
// 					}
// 					if ((c = _inbyte(DLY_1S)) >= 0 ) {
// 						switch (c) {
// 						case ACK:
// 							++packetno;
// 							len += xmodemState.currentPacketSize;
// 							goto start_trans;
// 						case CAN:
// 							if ((c = _inbyte(DLY_1S)) == CAN) {
// 								serial_out(ACK);
// 								flush_input();
// 								return -1; /* canceled by remote */
// 							}
// 							break;
// 						case NAK:
// 						default:
// 							break;
// 						}
// 					}
// 				}
// 				serial_out(CAN);
// 				serial_out(CAN);
// 				serial_out(CAN);
// 				flush_input();
// 				return -4; /* xmit error */
// 			}
// 			else {
// 				for (retry = 0; retry < 10; ++retry) {
// 					serial_out(EOT);
// 					if ((c = _inbyte((DLY_1S)<<1)) == ACK) break;
// 				}
// 				flush_input();
// 				return (c == ACK)?len:-5;
// 			}
// 		}
// 	}
// }
