
#include "wget.h"
#include "arguments.h"
#include "esp8266.h"
#include "print.h"
#include "utils.h"
#include <fossil.h>
#include <msxdos.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xmodem.h>

const unsigned char *pTempFileName = "xmdwn.tmp";
bool                 started;
uint32_t             totalFileSize;
const unsigned char *rotatingChar[4] = {"\x01\x56\x1B\x44", "\x01\x5D\x1B\x44", "\x01\x57\x1B\x44", "\x01\x5E\x1B\x44"};
uint8_t              rotatingIndex = 0;
FILE *               fptrDiagnostics;
const char *         defaultWaitForMessage = ERASE_LINE "Waiting for data ...";
char *               waitForMessage = NULL;

void subCommandWGet() {
  print_str("Attempting to retrieve file ");
  print_str(pFileName);
  print_str(" from ");
  print_str(pWgetUrl);
  print_str("\r\n");

  waitForMessage = (char *)defaultWaitForMessage;

  wget();
}

void wget() {
  started = false;
  totalFileSize = 0;
  print_str(ERASE_LINE "Connecting ...");

  fossil_rs_flush();
  fossil_rs_string("\r\nat+wget");
  if (requestLargePacket)
    fossil_rs_string("1");
  fossil_rs_string(pWgetUrl);
  fossil_rs_string("\r\n");

  if (fossil_rs_read_line(false) || strncmp(responseStr, "OK", 2) != 0) {
    print_str(ERASE_LINE "Resetting modem ...");
    resetModem();

    fossil_rs_flush();
    fossil_rs_string("\r\nat+wget");
    if (requestLargePacket)
      fossil_rs_string("1");
    fossil_rs_string(pWgetUrl);
    fossil_rs_string("\r\n");

    if (fossil_rs_read_line(false) || strncmp(responseStr, "OK", 2) != 0) {
      print_str("\r\nError requesting file:\r\n");
      print_str(responseStr);
      fossil_rs_flush_with_log();
      print_str("\r\n");
      abortWithError(NULL);
      return;
    }
  }

  print_str(waitForMessage);

  FILE *fptr = NULL;
  if (pFileName)
    fptr = fopen(pTempFileName, "wb");

#ifdef DIAGNOSTIC_FILE_CAPTURE
  fptrDiagnostics = fopen("esp8266.txt", "wb");
#endif

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
      if (pFileName)
        fwrite(xmodemState.packetBuffer + 3, xmodemState.currentPacketSize, 1, fptr);
      print_str(rotatingChar[rotatingIndex]);
      rotatingIndex = (rotatingIndex + 1) & 3;
    }

    if (sig & PACKET_TIMEOUT) {
      printf("\r\nTimeout %04X\r\n", sig);
    }

    if (sig & PACKET_REJECT) {
      printf("\r\nReject %04X\r\n", sig);
    }

    if (sig & TRY_AGAIN) {
      printf("\r\nTry Again %04X\r\n", sig);
    }

    if (sig & TOO_MANY_ERRORS) {
      printf("\r\nToo Many Errors %04X\r\n", sig);
    }

    if (sig & UPSTREAM_CANCELLED) {
      printf("\r\nUpstream Cancelled %04X\r\n", sig);
    }
  }

  if (xmodemState.finish_reason != END_OF_STREAM) {
    print_str(ERASE_LINE "Error receiving file\r\n");
    goto abort;
  }

  if (pFileName) {
    print_str(ERASE_LINE "Saving file...");
  }

#ifdef DIAGNOSTIC_FILE_CAPTURE
  fclose(fptrDiagnostics);
#endif

  if (pFileName) {
    fclose(fptr);
    remove(pFileName);
    rename(pTempFileName, pFileName);

    print_str(ERASE_LINE "Downloaded ");
    print_str(uint32_to_string(totalFileSize));
    print_str(" bytes.\r\n");
  }

  return;

abort:
#ifdef DIAGNOSTIC_FILE_CAPTURE
  fclose(fptrDiagnostics);
#endif
  if (pFileName) {
    fclose(fptr);
    remove(pTempFileName);
  }

  abortWithError(NULL);
}
