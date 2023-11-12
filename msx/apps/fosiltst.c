
#include "fossil.h"
// #include "xstdio.h"
#include <extbio.h>
#include <msxdos.h>
#include <stdio.h>

typedef struct {
  uint8_t *pHead;
  uint8_t *pTail;
  uint8_t  data[];
} rs232Buff;

uint8_t  __at(0xFB1A) RS_ERRORS;
uint8_t  __at(0xFB17) RS_DATCNT;
uint8_t  __at(0xFB1C) RS_ESTBLS; // RTSON:		EQU	$		; Bit boolean. (RS-232C)
uint8_t  __at(0xFB1B) RS_FLAGS;  // RS-232C bit flags
uint8_t *__at(0xFB18) RS_BUFEND;

rs232Buff *__at(0xFB04) RS_FCB;

void extern debugBreak(void);
void extern enableVdpInterrupts(void);
void extern disableVdpInterrupts(void);

void main(void) {
  bool stat;

  // this is deprecated - let confirm it does not modify driver if already installed
  extbio_fossil_install();

  printf("fossil_link should return 1 -> %d\r\n", fossil_link());

  fossil_link();

  uint16_t version = fossil_get_version();

  printf("Version %04X\r\n", version);

  fossil_init();

  uint16_t r = fossil_setchannel(0);
  printf("setchannel result %04X\r\n", r);

  uint16_t b = fossil_set_baud(7 * 256 + 7); // 19200
  printf("Baud %04X\r\n", b);

  fossil_rs_out('A');
  fossil_rs_out('T');
  fossil_rs_out('\r');
  fossil_rs_out('\n');

  stat = fossil_rs_in_stat();
  printf("sent AT.  Stat: %02X\r\n", stat);

  uint8_t i = 0;
  while (true) {
    if (msxbiosBreakX())
      break;

    stat           = fossil_rs_in_stat();
    uint16_t count = fossil_chars_in_buf();

    if (count > 0 || stat) {
      printf("count: %d, %d\r\n", count, stat);

      char ch = fossil_rs_in();
      printf(":%c\r\n", ch);
    }

    if (i++ % 16 == 0) {
      fossil_rs_out('1');
      fossil_rs_out('2');
      fossil_rs_out('3');
      fossil_rs_out('4');
    }
  }

exitApp:
  fossil_deinit();
}
