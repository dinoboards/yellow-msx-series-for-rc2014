
#include "extbio.h"
#include "msxdos.h"
#include "xstdio.h"
#include <stdio.h>

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

extern void     fossil_link(void *jumpTable) __z88dk_fastcall;
extern uint16_t fossil_get_version();
extern void     fossil_init();
extern void     fossil_deinit();
extern void     fossil_rs_out(char c) __z88dk_fastcall;
extern bool     fossil_rs_in_stat();
extern char     fossil_rs_in();

rs232Buff *__at 0xFB04 RS_FCB;

void extern debugBreak();

void main() {
  debugBreak();

  void *p = extbio_fossil_install();

  xprintf("Address %p\r\n", p);

  fossil_link(p);

  uint16_t version = fossil_get_version();

  xprintf("Version %04X\r\n", version);

  fossil_init();

  debugBreak();
  xprintf("BUF AT %p, head: %p, tail: %p\r\n", RS_FCB, RS_FCB->pHead, RS_FCB->pTail);
  fossil_rs_out('A');

  while (1) {
    uint16_t timeout = 32000;

    bool stat;

    if (msxbiosBreakX())
      goto exitApp;

    stat = fossil_rs_in_stat();

    while (!stat && timeout != 0) {
      if (timeout % 2048 == 0) {
        printf(".");
      }

      if (msxbiosBreakX())
        goto exitApp;

      stat = fossil_rs_in_stat();

      timeout--;
    }

    if (timeout == 0) {
      printf("\r\n");
    }

    while (stat) {
      printf(">> H: %p, T: %p, ST: %d,\r\n", RS_FCB->pHead, RS_FCB->pTail, stat);
      char ch = fossil_rs_in();
      printf(">> H: %p, T: %p, ST: %d, ch: %c\r\n", RS_FCB->pHead, RS_FCB->pTail, stat, ch);

      stat = fossil_rs_in_stat();
    }

    fossil_rs_out('A');
    fossil_rs_out('B');
    fossil_rs_out('C');
    fossil_rs_out('D');
  }

exitApp:
  printf("RS_FLAGS: %02X\r\nClosing...\r\n", RS_FLAGS);
  fossil_deinit();
  printf("RS_FLAGS: %02X\r\nClosed\r\n", RS_FLAGS);
}
