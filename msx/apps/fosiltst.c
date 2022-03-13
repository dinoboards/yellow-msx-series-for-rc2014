
#include "fossil.h"
#include "xstdio.h"
#include <extbio.h>
#include <msxdos.h>
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

rs232Buff *__at 0xFB04 RS_FCB;

void extern debugBreak();
void extern enableVdpInterrupts();
void extern disableVdpInterrupts();

void main() {
  printf("fossil_link should return false before install %d\r\n", fossil_link());

  void *p = extbio_fossil_install();

  printf("fossil_link should return true after %d\r\n", fossil_link());

  xprintf("Address %p\r\n", p);

  fossil_link();

  uint16_t version = fossil_get_version();

  xprintf("Version %04X\r\n", version);

  uint16_t b = fossil_set_baud(7 * 256 + 7); // 19200
  printf("Baud %04X\r\n", b);

  fossil_init();

  xprintf("BUF AT %p, head: %p, tail: %p\r\n", RS_FCB, RS_FCB->pHead, RS_FCB->pTail);
  fossil_rs_out('A');

  printf("Send A\r\n");
  while (1) {
    uint16_t timeout = 32000;

    printf(">");
    fossil_rs_out('T');
    printf("<");

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

      printf(">");
      fossil_rs_out('T');
      printf("<");

      timeout--;
    }

    if (timeout == 0) {
      printf("\r\n");
    }

    while (stat) {
      uint16_t count = fossil_chars_in_buf();
      // printf(">> H: %p, T: %p, ST: %d\r\n", RS_FCB->pHead, RS_FCB->pTail, stat);
      // printf(">> LN: %d ", count);
      char ch = fossil_rs_in();
      printf("%c", ch);
      // printf(">> H: %p, T: %p, ST: %d, ch: %c,\r\n", RS_FCB->pHead, RS_FCB->pTail, stat, ch);

      if (msxbiosBreakX())
        goto exitApp;

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
