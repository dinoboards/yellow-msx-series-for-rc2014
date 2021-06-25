#ifndef __EXTBIO
#define __EXTBIO

#include <stdlib.h>

typedef struct {
  char     character_length;    // Character length '5'-'8'
  char     parity;              // Parity 'E','O','I','N'
  char     stop_bits;           // Stop bits '1','2','3'
  char     xon_xoff_control;    // XON/XOFF controll 'X','N'
  char     handshake;           // CTR-RTS hand shake 'H','N'
  char     autoLineFeedSend;    // Auto LF for receive 'A','N'
  char     autoLineFeedReceive; // Auto LF for send 'A','N'
  char     si_so_control;       // SI/SO control 'S','N'
  uint16_t receive_baud;        // 50-19200
  uint16_t send_baud;           // 50-19200
  uint8_t  timeout_counter;     // Time out counter 0-255
} rs232_init_params;

typedef struct {
  uint8_t slot_id;
  void *  jump_table;
  uint8_t reserved;
} extbio_info;

typedef struct {
  uint8_t deviceId;
  uint8_t _reserved;
} extbio_device_table;

extern void                 extbio_get_dev_info_table(uint8_t device_id, extbio_info *info_table);
extern extbio_device_table *extbio_get_dev(extbio_device_table *table) __z88dk_fastcall;

// RC2014 DRIVER

extern void *extbio_fossil_install();

// RS232 DRIVER
#define RS232_RAW_MODE 4

extern void     rs232_link(extbio_info *p) __z88dk_fastcall;
extern void     rs232_init(rs232_init_params *) __z88dk_fastcall;
extern void     rs232_open(uint8_t mode, uint8_t buffer_length, uint8_t *buffer);
extern void     rs232_close();
extern uint8_t  rs232_sndchr(const char ch) __z88dk_fastcall;
extern uint16_t rs232_getchr();
extern uint16_t rs232_loc();

#endif
