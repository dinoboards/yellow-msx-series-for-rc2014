#include "arguments.h"
#include "fossil.h"
#include "msxdos.h"
#include "print.h"
#include "utils.h"
#include "xmodem.h"
#include <extbio.h>
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
 * [x] Minimise code size if possible
 * [ ] Improved error handling reporting
 * [x] Show file size download progress
 * [x] Report file size during download and at completion
 * [x] Auto activate fossil driver
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

const unsigned char *pTempFileName = "xmdwn.tmp";

const unsigned char *rotatingChar[4] = {"\x01\x56\x1B\x44", "\x01\x5D\x1B\x44", "\x01\x57\x1B\x44", "\x01\x5E\x1B\x44"};
uint8_t              rotatingIndex = 0;
bool                 started = false;
uint32_t             totalFileSize = 0;

#define ERASE_LINE "\x1B\x6C\r"

int main(const int argc, const unsigned char **argv) {
  if (!fossil_link()) {
    extbio_fossil_install();

    if (!fossil_link()) {
      print_str("Fossil driver not found\r\n");
      exit(1);
    }
  }

  process_cli_arguments(argv, argc);

  FILE *fptr;

  print_str("MSX/RC2014 Xmodem receive v0.5.1\r\n");

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

  print_str("Press CTRL-BREAK to abort\r\n");
  print_str("Waiting for file to be sent ...");

  XMODEM_SIGNAL sig = READ_FIRST_HEADER;
  while (sig = xmodem_receive(sig)) {
    if (msxbiosBreakX())
      goto abort;

    if (!started && (sig & (READ_128 | READ_1024))) {
      started = true;
      print_str(ERASE_LINE "Downloading ");
      print_str(sig & READ_CRC ? "(crc) ... " : "(chksum) ... ");
    }

    if (sig & SAVE_PACKET) {
      totalFileSize += xmodemState.currentPacketSize;
      fwrite(xmodemState.packetBuffer + 3, xmodemState.currentPacketSize, 1, fptr);
      print_str(rotatingChar[rotatingIndex]);
      rotatingIndex = (rotatingIndex + 1) & 3;
    }
  }

  if (xmodemState.finish_reason != END_OF_STREAM) {
    print_str(ERASE_LINE "Error receiving file\r\n");
    goto abort;
  }

  print_str(ERASE_LINE "Saving file...");

  fossil_deinit();
  fclose(fptr);
  remove(pFileName);
  rename(pTempFileName, pFileName);

  print_str(ERASE_LINE "Downloaded ");
  print_str(uint32_to_string(totalFileSize));
  print_str(" bytes.\r\n");
  return 0;

abort:
  fossil_deinit();
  fclose(fptr);
  remove(pTempFileName);
  return 1;
}
