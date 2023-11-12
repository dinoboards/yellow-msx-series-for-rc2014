#ifndef __FOSSIL_INTERFACE
#define __FOSSIL_INTERFACE

#include <stdbool.h>
#include <stdlib.h>

// Offset	Bytes	Description
// ;	+0	2	Version number +1 contains main version
// ;			+0 contains sub-version, both packed BCD
// ;	+2	1	Current receive speed index
// ;	+3	1	Current send speed index
// ;	+4	1	Current protocol
// ;			bit 0-1 Data Bits
// ;				00 5 bits or less
// ;				01 6 bits
// ;				10 7 bits
// ;				11 8 bits
// ;			    2-3 Stop Bits
// ;				00 (SYNC modes enable)
// ;				01 1 stopbit
// ;				10 1.5 stopbits
// ;				11 2 stopbits
// ;			    4-5 Parity
// ;				00 none
// ;				01 even
// ;				11 odd
// ;			    6-7 0 (not defined yet)
// ;	+5	1	ChPut_Hook redirection status
// ;	+6	1	Keyboard_Hook redirection status
// ;	+7	1	Current RTS status (Request To Send, or NOT!)
// ;	+8	1	Current DTR status (Data Terminal Ready)
// ;	+9	1	Current channel
// ;	+10	1	Hardware info
// ;			0 = No description
// ;			1 = ASCII monochannel compatible
// ;			2 = NMS121x interface (multichannel)
// ;			4 = NMS1250, adapted for RS232C
// ;			8 = 16550 UART with FIFO (Sunrise RS232C + ACC

typedef struct fossil_info_struct {
  uint16_t version_number;
  uint8_t  rx_speed_index;
  uint8_t  tx_speed_index;
  uint8_t  protocol;
  uint8_t  chput_hook_status;
  uint8_t  keyb_hook_status;
  uint8_t  rts_status;
  uint8_t  dtr_status;
  uint8_t  channel;
  uint8_t  hardware;
} fossil_info;

typedef struct {
  uint8_t slot_id;
  void   *jump_table;
} fossil_jump_table;

extern uint16_t      fossil_get_version(void);
extern bool          fossil_link(void);
extern uint8_t       fossil_ex_link(fossil_jump_table *jtable) __z88dk_fastcall;
extern int           fossil_getversion(void);
extern uint16_t      fossil_init(void);
extern void          fossil_deinit(void);
extern uint16_t      fossil_set_baud(uint16_t transmit_and_receive_baud) __z88dk_fastcall;
extern void          fossil_set_protocol(uint8_t x) __z88dk_fastcall;
extern uint16_t      fossil_setchannel(uint8_t x) __z88dk_fastcall;
extern unsigned char fossil_rs_in(void);
extern void          fossil_rs_out(uint8_t ch) __z88dk_fastcall;
extern uint8_t       fossil_rs_in_stat(void);
extern void          fossil_rs_out_stat(void);
extern void          fossil_dtr(uint8_t x) __z88dk_fastcall;
extern void          fossil_rts(uint8_t x) __z88dk_fastcall;
extern uint8_t       fossil_carrier(void);
extern uint16_t      fossil_chars_in_buf(void);
extern uint16_t      fossil_size_of_buf(void);
extern void          fossil_flushbuf(void);
extern void          fossil_fastint(uint8_t x) __z88dk_fastcall;
extern void          fossil_hookstat(uint8_t x) __z88dk_fastcall;
extern void          fossil_chput_hook(uint8_t x) __z88dk_fastcall;
extern void          fossil_keyb_hook(uint8_t x) __z88dk_fastcall;
extern fossil_info  *fossil_get_info(void);

extern void fossil_rs_string(const char *pString);
extern void fossil_rs_flush(void);

#endif
