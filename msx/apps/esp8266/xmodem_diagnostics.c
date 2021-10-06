#define DIAGNOSTIC_FILE_CAPTURE true
#include "../xrecv2/xmodem.c"
#include <stdio.h>

extern FILE *fptrDiagnostics;

void serial_out(uint8_t ch) __z88dk_fastcall {
  fprintf(fptrDiagnostics, "> %04X %c\r\n", JIFFY, ch);
  return fossil_rs_out(ch);
}

unsigned char serial_in() {
  const unsigned char r = fossil_rs_in();
  fputc(r, fptrDiagnostics);
  return r;
}

void serial_flush_input(uint16_t period) __z88dk_fastcall {
  serial_in_block_start("\r\nflush\r\n");

  while (wait_for_byte(period))
    serial_in();

  serial_in_block_end("\r\n");
}

void serial_in_block_start(const unsigned char *pMessage) {
  if (pMessage)
    fprintf(fptrDiagnostics, pMessage);
  fprintf(fptrDiagnostics, "< %04X ", JIFFY);
}

void serial_in_block_end(const unsigned char *pMessage) {
  if (pMessage)
    fprintf(fptrDiagnostics, pMessage);
}

extern void serial_diagnostic_message(const unsigned char *pMessage) { fprintf(fptrDiagnostics, pMessage); }
