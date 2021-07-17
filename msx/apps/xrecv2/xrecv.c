#include "arguments.h"
#include "crt_override.h"
#include "fossil.h"
#include "msx.h"
#include "msxdos.h"
#include "print.h"
#include "xmodem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * TODO:
 * [x] Clean up copy on start
 * [x] Update usage help copy
 * [x] Print actual selected baud rate
 * [x] Monitor for CTRL+BREAK
 * [ ] Monitor for CTR+C to abort
 * [x] Dont create/open file is any errors - maybe create a tmp and then rename?
 * [ ] Minimise code size if possible
 * [ ] Improved error handling reporting
 * [ ] Show file size download progress
 * [ ] Auto activate fossil driver
 */

#define VDP_FREQUENCY 50

#define LF    ('J' - 0x40)
#define CR    ('M' - 0x40)
#define SOH   ('A' - 0x40)
#define EOT   ('D' - 0x40)
#define ACK   ('F' - 0x40)
#define NAK   ('U' - 0x40)
#define CAN   ('X' - 0x40)
#define A_SUB ('Z' - 0x40)

#define TIMEDOUT 0xFF
#define ERR1     0xFE
#define ERR2     0xFD
#define ERR3     0xFC
#define ERR4     0xFB
#define ERR5     0xFA
#define OK       0x00

const char *pTempFileName = "xmdwn.tmp";

int _main(const char **argv, const int argc) {
  if (!fossil_link()) {
    print_str("Fossil driver not found\r\n");
    exit(1);
  }

  process_cli_arguments(argv, argc);

  FILE *fptr;

  print_str("Filename: ");
  print_str((char *)pFileName);
  print_str("\r\n");
  print_str("Requested Baud Rate: ");
  print_str(fossil_rate_to_string(baud_rate));
  print_str("\r\n");
  uint16_t actual_baud_rate = fossil_set_baud(baud_rate);
  if (actual_baud_rate != baud_rate) {
    print_str("Actual Baud Rate: ");
    print_str(fossil_rate_to_string(actual_baud_rate));
    print_str("\r\n");
  }
  fptr = fopen(pTempFileName, "wb");

  fossil_set_protocol(7); // 8N1
  fossil_init();

  print_str("Awaiting for file to be received...\r\n");

  XMODEM_SIGNAL sig = READ_FIRST_HEADER;
  while (sig = xmodem_receive(sig)) {
    if (msxbiosBreakX()) {
      print_str("\r\nAborting\r\n");
      goto abort;
    }

    if (sig & SAVE_PACKET) {
      fwrite(xmodemState.packetBuffer + 3, xmodemState.currentPacketSize, 1, fptr);
      fputc_cons('.');
    }
  }

  if (xmodemState.finish_reason != END_OF_STREAM) {
    print_str("Error receiving file\r\n");
    goto abort;
  }

  fossil_deinit();
  fclose(fptr);
  rename(pTempFileName, pFileName);

  return 0;

abort:
  fossil_deinit();
  fclose(fptr);
  remove(pTempFileName);
  return 1;
}
