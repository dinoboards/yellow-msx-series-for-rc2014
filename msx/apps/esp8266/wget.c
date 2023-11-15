
#include "wget.h"
#include "arguments.h"
#include "esp8266.h"
#include "print.h"
#include <fossil.h>
#include <msxdos.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <system_vars.h>
#include <unistd.h>
#include <xmodem/utils.h>
#include <xmodem/xmodem.h>

const unsigned char pTempFileName[] = "xmdwn.tmp";
bool                started;
uint32_t            totalFileSize;
const unsigned char rotatingChar[4][5] = {"\x01\x56\x1B\x44", "\x01\x5D\x1B\x44", "\x01\x57\x1B\x44", "\x01\x5E\x1B\x44"};
uint8_t             rotatingIndex      = 0;
uint8_t             fptrDiagnostics;
const char          defaultWaitForMessage[] = ERASE_LINE "Waiting for data ...";
char               *waitForMessage          = NULL;
char                backupPacket[1025];
int16_t             backupPacketSize;
bool                firstPacket;

char *strnstr(const char *haystack, const char *needle, size_t len) {
  int    i;
  size_t needle_len;

  if (0 == (needle_len = strnlen(needle, len)))
    return (char *)haystack;

  for (i = 0; i <= (int)(len - needle_len); i++) {
    if ((haystack[0] == needle[0]) && (0 == strncmp(haystack, needle, needle_len)))
      return (char *)haystack;

    haystack++;
  }
  return NULL;
}

void subCommandWGet(void) {
  print_str("Attempting to retrieve file ");
  print_str(pFileName);
  print_str(" from ");
  print_str(pWgetUrl);
  print_str("\r\n");

  waitForMessage = (char *)defaultWaitForMessage;

  wget();
}

void wget(void) {
  xmodem_enable_extended_info_packet_support();
  firstPacket   = true;
  started       = false;
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

  char downloadMessage[3 + 13 + 1];
  strcpy(downloadMessage, ERASE_LINE);
  if (pFileName)
    strcat(downloadMessage, pFileName);
  else
    strcat(downloadMessage, "Downloading");

  strcat(downloadMessage, " ");

  uint8_t fptr = 0;
  if (pFileName) {
    const uint16_t error_handle = msxdosCreateFile(pTempFileName, IO_WRONLY);
    const uint8_t  error        = error_handle >> 8 & 0xFF;
    if (error) {
      char error_description[64];
      memset(error_description, 0, sizeof(error_description));
      msxdosExplainErrorCode(error, error_description);
      printf("%s (%02X): %s.\r\n", error_description, error, pFileName);
      return;
    }

    fptr = error_handle & 0xFF;
  }

  const int16_t startTime = JIFFY;

  XMODEM_SIGNAL sig = READ_FIRST_HEADER;
  while (sig = xmodem_receive(sig)) {
    if (msxbiosBreakX())
      goto abort;

    if (sig & SAVE_PACKET && !(sig & INFO_PACKET)) {
      if (!firstPacket && pFileName)
        msxdosWriteFile(fptr, backupPacket, backupPacketSize);

      print_str(downloadMessage);
      print_str(sig & READ_CHECKSUM ? "(chksum) ... " : "(crc) ... ");

      firstPacket      = false;
      backupPacketSize = xmodemState.currentPacketSize;
      memcpy(backupPacket, xmodemState.packetBuffer + 3, backupPacketSize);
      totalFileSize += backupPacketSize;
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

  if (!(xmodemState.finish_reason | END_OF_STREAM)) {
    print_str(ERASE_LINE "Error receiving file\r\n");
    goto abort;
  }

  if (xmodemState.finish_reason | INFO_PACKET) {
    const char *lengthPtr = strnstr(xmodemState.packetBuffer + 3, "LEN=", 128);
    if (lengthPtr) {
      const int32_t length = atol(lengthPtr + 4);

      backupPacketSize = length - (totalFileSize - backupPacketSize);
      totalFileSize    = length;
    }
  }

  if (pFileName) {
    msxdosWriteFile(fptr, backupPacket, backupPacketSize);
    print_str(ERASE_LINE "Saving file...");
  }

  if (pFileName) {
    const int16_t  ticks     = JIFFY - startTime;
    const float    totalTime = ((float)ticks / (float)GET_VDP_FREQUENCY());
    const uint32_t rate      = (float)totalFileSize / totalTime;

    msxdosCloseFile(fptr);
    remove(pFileName);
    rename(pTempFileName, pFileName);

    print_str(ERASE_LINE);
    if (pFileName)
      print_str(pFileName);
    else
      print_str("Downloaded");

    printf(" %ld bytes in %d seconds (%d b/s)\r\n", totalFileSize, (int)totalTime, (int)rate);
  }

  return;

abort:
  if (pFileName) {
    msxdosCloseFile(fptr);
    remove(pTempFileName);
  }

  abortWithError(NULL);
}
