#include "xmodem.h"
#include "crc16.h"
#include <extbio/serial-helpers.h>
#include <extbio/serial.h>
#include <stdio.h>
#include <string.h>
#include <system_vars.h>
#include <z80.h>

#define SOH   0x01
#define STX   0x02
#define EOT   0x04
#define ACK   0x06
#define NAK   0x15
#define CAN   0x18
#define CTRLZ 0x1A
#define RS    0x1e

static const char three_can_bytes[] = {CAN, CAN, CAN};

#define DLY_1S     (VDP_FREQUENCY * 2)
#define MAXRETRANS 25
#define TRANSMIT_XMODEM_1K

#define FOR_SIGNAL(mask) if (signal & (mask))

extern uint8_t port_number;

struct xmodemState xmodemState;
unsigned char      packetno = 1;
uint16_t           i;
uint16_t           retry, retrans = MAXRETRANS;

bool supportExtendedInfoPacket = false;

static int16_t       delay_point;
static XMODEM_SIGNAL delay_resume;

inline XMODEM_SIGNAL delay_start(uint16_t period, XMODEM_SIGNAL next_signal) {
  delay_point  = ((int16_t)JIFFY) + (period);
  delay_resume = (next_signal);
  return DELAY_WAIT;
}

XMODEM_SIGNAL delay_reached(void) {
  EI;
  return ((delay_point - ((int16_t)JIFFY) >= 0)) ? DELAY_WAIT : delay_resume;
}

static bool check_crc(void) {
  const unsigned char *buf = &xmodemState.packetBuffer[3];
  const uint16_t       sz  = xmodemState.currentPacketSize;
  uint16_t             counter;
  uint16_t             crc = 0;
  for (counter = 0; counter < sz; counter++)
    crc = (crc << 8) ^ crc16tab[((crc >> 8) ^ *buf++) & 0x00FF];

  const uint16_t tcrc = (*buf++ << 8) + *buf;
  return (crc == tcrc);
}

static bool check_sum(void) {
  const unsigned char *buf = &xmodemState.packetBuffer[3];
  const uint16_t       sz  = xmodemState.currentPacketSize;

  uint8_t cks = 0;
  for (uint16_t i = 0; i < sz; ++i) {
    cks += *buf++;
  }
  return (cks == *buf);
}

bool read_packet_crc(void) {
  unsigned char *p = xmodemState.packetBuffer + 1;

  const uint16_t expected_size = xmodemState.currentPacketSize + 1 + 3;
  uint16_t       actual_size   = expected_size;

  serial_demand_read(port_number, p, &actual_size, 1000);

  return expected_size == actual_size;
}

bool read_packet_sum(void) {
  unsigned char *p = xmodemState.packetBuffer + 1;

  const uint16_t expected_size = xmodemState.currentPacketSize + 0 + 3;
  uint16_t       actual_size   = expected_size;

  serial_demand_read(port_number, p, &actual_size, DLY_1S);

  return expected_size == actual_size;
}

XMODEM_SIGNAL read_first_header(void) {
  packetno = 1;
  retry = retrans = MAXRETRANS;

  serial_write_char('C');

  uint8_t x;
  if ((serial_read_char(&x) == 0))
    switch (x) {
    case SOH:
      xmodemState.currentPacketSize = 128;
      return READ_CRC | READ_128;

    case STX:
      xmodemState.currentPacketSize = 1024;
      return READ_CRC | READ_1024;

    case EOT: {
      serial_write_char(ACK);
      return END_OF_STREAM;
    }

    case CAN: {
      serial_purge_buffers(port_number);
      serial_write_char(ACK);
      return UPSTREAM_CANCELLED;
    }
    }

  return delay_start(DLY_1S * 4, READ_FIRST_HEADER | TRY_AGAIN);
}

XMODEM_SIGNAL read_header(const XMODEM_SIGNAL signal) __z88dk_fastcall {
  uint8_t x;

  if (serial_read_char(&x) == 0) {
    switch (x) {
    case SOH:
      xmodemState.currentPacketSize = 128;
      return signal | READ_128;

    case STX:
      xmodemState.currentPacketSize = 1024;
      return signal | READ_1024;

    case RS:
      return signal | READ_128 | INFO_PACKET;

    case EOT: {
      if (supportExtendedInfoPacket) {
        packetno = 0;
        serial_write_char(ACK);
        return read_header(signal & (READ_CRC | READ_CHECKSUM));
      }
      serial_write_char(ACK);
      return END_OF_STREAM;
    }

    case CAN: {
      serial_purge_buffers(port_number);
      serial_write_char(ACK);
      return UPSTREAM_CANCELLED;
    }
    }
  }

  return delay_start(DLY_1S * 4, signal | TRY_AGAIN);
}

XMODEM_SIGNAL start_receive_crc(const XMODEM_SIGNAL signal) __z88dk_fastcall {
  if (read_packet_crc())
    if (xmodemState.packetBuffer[1] == (unsigned char)(~xmodemState.packetBuffer[2]) && (xmodemState.packetBuffer[1] == packetno) &&
        check_crc()) {
      return signal | SAVE_PACKET;
    }

  if (--retrans <= 0) {
    serial_purge_buffers(port_number);
    serial_write_bytes(three_can_bytes, 3);
    return TOO_MANY_ERRORS;
  }

  return delay_start(DLY_1S * 4, signal | PACKET_REJECT);
}

XMODEM_SIGNAL start_receive_checksum(const XMODEM_SIGNAL signal) __z88dk_fastcall {
  if (!read_packet_sum())
    return delay_start(DLY_1S * 4, signal | PACKET_TIMEOUT | STREAM_ERROR);

  if (xmodemState.packetBuffer[1] == (unsigned char)(~xmodemState.packetBuffer[2]) && (xmodemState.packetBuffer[1] == packetno) &&
      check_sum())
    return signal | SAVE_PACKET;

  if (--retrans <= 0) {
    serial_purge_buffers(port_number);
    serial_write_bytes(three_can_bytes, 3);
    return TOO_MANY_ERRORS;
  }

  return delay_start(DLY_1S * 4, signal | PACKET_REJECT);
}

XMODEM_SIGNAL xmodem_tryagain(const XMODEM_SIGNAL signal) __z88dk_fastcall {
  serial_purge_buffers(port_number);

  if (retry++ >= 5)
    return TOO_MANY_ERRORS;

  fputc_cons('.');
  serial_write_char(NAK);
  retry++;
  return signal | READ_HEADER;
}

XMODEM_SIGNAL xmodem_packet_save(const XMODEM_SIGNAL signal) __z88dk_fastcall {
  serial_write_char(ACK);
  ++packetno;
  retrans = MAXRETRANS + 1;
  retry   = 0;
  return signal | READ_HEADER;
}

XMODEM_SIGNAL xmodem_too_many_errors(void) __z88dk_fastcall {
  serial_purge_buffers(port_number);
  serial_write_bytes(three_can_bytes, 3);
  xmodemState.finish_reason = TOO_MANY_ERRORS;
  return FINISHED;
}

XMODEM_SIGNAL xmodem_receive(const XMODEM_SIGNAL signal) __z88dk_fastcall {
  FOR_SIGNAL(READ_128 | READ_1024) {
    FOR_SIGNAL(READ_CRC) { return start_receive_crc(signal & ~(READ_128 | READ_1024)); }
    else {
      return start_receive_checksum(signal & ~(READ_128 | READ_1024));
    }
  }

  FOR_SIGNAL(DELAY_WAIT) { return delay_reached(); }

  FOR_SIGNAL(READ_FIRST_HEADER) { return read_first_header(); }

  FOR_SIGNAL(READ_HEADER) { return read_header(signal & ~READ_HEADER); }

  FOR_SIGNAL(PACKET_TIMEOUT | PACKET_REJECT | TRY_AGAIN) {
    return xmodem_tryagain(signal & ~(PACKET_TIMEOUT | PACKET_REJECT | TRY_AGAIN));
  }

  FOR_SIGNAL(INFO_PACKET) {
    FOR_SIGNAL(SAVE_PACKET) {
      serial_write_char(ACK);
      return INFO_PACKET | END_OF_STREAM;
    }
  }

  FOR_SIGNAL(SAVE_PACKET) { return xmodem_packet_save(signal & ~SAVE_PACKET); }

  FOR_SIGNAL(TOO_MANY_ERRORS) { return xmodem_too_many_errors(); }

  xmodemState.finish_reason = signal;
  return FINISHED;
}
