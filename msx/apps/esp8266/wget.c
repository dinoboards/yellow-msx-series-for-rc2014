
#include "wget.h"
#include "arguments.h"
#include "esp8266.h"
#include "print.h"
// #include <fossil.h>
#include <extbio/serial-helpers.h>
#include <extbio/serial.h>
#include <msx/libgen.h>
#include <msxdos.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <system_vars.h>
#include <xmodem/utils.h>
#include <xmodem/xmodem.h>

bool                started;
uint32_t            totalFileSize;
const unsigned char rotatingChar[4][5]      = {"\x01\x56\x1B\x44", "\x01\x5D\x1B\x44", "\x01\x57\x1B\x44", "\x01\x5E\x1B\x44"};
uint8_t             rotatingIndex           = 0;
const char          defaultWaitForMessage[] = ERASE_LINE "Waiting for data ...";
char               *waitForMessage          = NULL;
bool                firstPacket;

char    backupPacket[1025];
int16_t backupPacketSize;

char serial_write_temp_buffer[256];

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
  print_str(pFilePathName);
  print_str(" from ");
  print_str(pWgetUrl);
  print_str("\r\n");

  waitForMessage = (char *)defaultWaitForMessage;

  wget();
}

uint16_t error_handle;
uint8_t  error;
char    *pPath;
char    *pFileName;
char     fileNameBuffer[256];
char     pTempFileName[256];

void wget(void) {
  xmodem_enable_extended_info_packet_support();
  firstPacket   = true;
  started       = false;
  totalFileSize = 0;
  print_str(ERASE_LINE "Connecting ...");

  serial_purge_buffers(port_number);
  serial_write_string("\r\nat+wget");
  if (requestLargePacket)
    serial_write_string("1");
  serial_write_string(pWgetUrl);
  serial_write_string("\r\n");

  if (serial_read_line(false) || strncmp(responseStr, "OK", 2) != 0) {
    print_str(ERASE_LINE "Resetting modem ...");
    resetModem();

    serial_purge_buffers(port_number);
    serial_write_string("\r\nat+wget");
    if (requestLargePacket)
      serial_write_string("1");
    serial_write_string(pWgetUrl);
    serial_write_string("\r\n");

    if (serial_read_line(false) || strncmp(responseStr, "OK", 2) != 0) {
      print_str("\r\nError requesting file:\r\n");
      print_str(responseStr);
      serial_purge_buffers(port_number);
      print_str("\r\n");
      abortWithError(NULL);
      return;
    }
  }

  print_str(waitForMessage);

  char downloadMessage[3 + 13 + 1];
  strcpy(downloadMessage, ERASE_LINE);
  if (pFilePathName)
    strcat(downloadMessage, pFilePathName);
  else
    strcat(downloadMessage, "Downloading");

  strcat(downloadMessage, " ");

  uint8_t fptr = 0;
  if (pFilePathName) {

    strcpy(fileNameBuffer, pFilePathName);
    pPath     = dirname(fileNameBuffer);
    pFileName = basename(pFilePathName);
    sprintf(pTempFileName, "%sxmdwn.tmp", pPath, pFileName);

    error_handle = msxdosCreateFile(pTempFileName, IO_NO_RD);
    error        = error_handle >> 8 & 0xFF;
    if (error) {
      char error_description[64];
      memset(error_description, 0, sizeof(error_description));
      msxdosExplainErrorCode(error, error_description);
      printf("%s (%02X): %s.\r\n", error_description, error, pTempFileName);
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
      if (!firstPacket && pFilePathName) {
        error = msxdosWriteFile(fptr, backupPacket, backupPacketSize);
        if (error) {
          char error_description[64];
          memset(error_description, 0, sizeof(error_description));
          msxdosExplainErrorCode(error, error_description);
          printf("%s (%02X): %s.\r\n", error_description, error, pFilePathName);
          return;
        }
      }

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

  if (pFilePathName) {
    msxdosWriteFile(fptr, backupPacket, backupPacketSize);
    print_str(ERASE_LINE "Saving file...");
  }

  if (pFilePathName) {
    const int16_t  ticks     = JIFFY - startTime;
    const float    totalTime = ((float)ticks / (float)GET_VDP_FREQUENCY());
    const uint32_t rate      = (float)totalFileSize / totalTime;

    msxdosCloseFile(fptr);
    msxdosDeleteFile(pFilePathName);
    msxdosRenameFile(pFilePathName, pTempFileName);

    print_str(ERASE_LINE);
    if (pFilePathName)
      print_str(pFilePathName);
    else
      print_str("Downloaded");

    printf(" %ld bytes in %d seconds (%d b/s)\r\n", totalFileSize, (int)totalTime, (int)rate);
  }

  return;

abort:
  if (pFilePathName) {
    msxdosCloseFile(fptr);
    msxdosDeleteFile(pTempFileName);
  }

  abortWithError(NULL);
}
