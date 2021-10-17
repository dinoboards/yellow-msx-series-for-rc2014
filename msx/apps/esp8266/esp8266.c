#include "features.h"

#include "arguments.h"
#include "print.h"
#include <ctype.h>
#include <delay.h>
#include <extbio.h>
#include <fossil.h>
#include <msxdos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <system_vars.h>
#include <utils.h>
#include <xmodem.h>

#define ONE_SECOND              (VDP_FREQUENCY * 1.1)
#define MAX_RESPONSE_STRING_LEN 128
unsigned char responseStr[MAX_RESPONSE_STRING_LEN + 1];

#if 1
void fossil_rs_flush_with_log() {
  int16_t timeout = get_future_time(from_ms(5000));

  while (!is_time_past(timeout)) {
    while (fossil_rs_in_stat() != 0) {
      unsigned char c = fossil_rs_in();
      printf("%c", c);
      timeout = get_future_time(from_ms(5000));
    }
  }
}
#endif

bool fossil_rs_read_line(const bool withLogging) {
  uint8_t        length = 0;
  int16_t        timeout = get_future_time(from_ms(5000));
  unsigned char *pBuffer = responseStr;

  while (length < MAX_RESPONSE_STRING_LEN && !is_time_past(timeout)) {
    if (fossil_rs_in_stat() != 0) {
      timeout = get_future_time(from_ms(5000));
      const unsigned char t = fossil_rs_in();
      if (withLogging)
        fputc_cons(t);

      if (t >= 32 && t < 128) {
        *pBuffer++ = t;
        length++;
      }

      if (t == '\n')
        break;
    }
  }

  *pBuffer = 0;

  return is_time_past(timeout);
}

bool read_until_ok_or_error() {
  int16_t timeout = get_future_time(from_ms(2000));
  bool    lineReceived = true;

  responseStr[0] = 0;

  while (true) {
    if (lineReceived = fossil_rs_read_line(true)) {
      timeout = get_future_time(from_ms(2000));
      continue;
    }

    if ((strncmp(responseStr, "OK", 2) != 0 && strncmp(responseStr, "ERROR", 5) != 0) && !is_time_past(timeout))
      continue;

    break;
  }

  return strncmp(responseStr, "OK", 2) == 0;
}

void subCommandTimeSync() {
  fossil_rs_flush();
  fossil_rs_string("\r\nAT+time?\r\n");
  if (fossil_rs_read_line(false)) {
    print_str("Error getting time\r\n");
    return;
  }

  if (strlen(responseStr) != 24) {
    print_str("Invalid time string received ");
    print_str(responseStr);
    print_str("\r\n");
    return;
  }

  const int year = atoi(responseStr);
  const int month = atoi(responseStr + 5);
  const int day = atoi(responseStr + 8);
  const int hour = atoi(responseStr + 11);
  const int min = atoi(responseStr + 14);
  const int sec = atoi(responseStr + 17);

  uint8_t currentHour;
  uint8_t currentMinute;
  uint8_t currentSecond;
  msxdosGetTime(&currentHour, &currentMinute, &currentSecond);

  msxdosSetDate(year, month, day);
  msxdosSetTime(hour, min, sec, 0);

  int32_t t1 = (((int32_t)currentHour) * 60 * 60) + (int32_t)currentMinute * 60 + (int32_t)currentSecond;
  int32_t t2 = ((int32_t)hour * 60 * 60) + (int32_t)min * 60 + (int32_t)sec;

  printf("Time: %04d:%02d:%02d %02d:%02d:%02d\r\n", year, month, day, hour, min, sec);
}

void subCommandSetTimeZone() {
  fossil_rs_flush();
  fossil_rs_string("\r\nat+locale=");
  fossil_rs_string(pNewTimeZone);
  fossil_rs_string("\r\n");

  if (fossil_rs_read_line(false)) {
    print_str("Error setting timezone\r\n");
    return;
  }

  if (strncmp(responseStr, "OK", 2) == 0) {
    subCommandTimeSync();
    return;
  }

  print_str("Error setting timezone: ");
  print_str(responseStr);
  print_str("\r\n");
}

void subCommandSetWiFi() {
  fossil_rs_flush();
  fossil_rs_string("\r\nat+cwjap=");
  fossil_rs_string(pNewSSID);
  fossil_rs_string(",");
  fossil_rs_string(pNewPassword);
  fossil_rs_string("\r\n");

  read_until_ok_or_error();
}

#define ERASE_LINE "\x1B\x6C\r"
const unsigned char *pTempFileName = "xmdwn.tmp";
bool                 started = false;
uint32_t             totalFileSize = 0;
const unsigned char *rotatingChar[4] = {"\x01\x56\x1B\x44", "\x01\x5D\x1B\x44", "\x01\x57\x1B\x44", "\x01\x5E\x1B\x44"};
uint8_t              rotatingIndex = 0;
FILE *               fptrDiagnostics;

void resetModem() {
  delay(ONE_SECOND);
  fossil_rs_string("+++");
  delay(ONE_SECOND);
  fossil_rs_string("\r\nATH\r\n"); // hang up
  fossil_rs_read_line(false);
  fossil_rs_string("\r\nATE0\r\n"); // echo off
  fossil_rs_read_line(false);
}

extern uint8_t wget();

void subCommandWGet() {
  print_str("Attempting to retrieve file ");
  print_str(pFileName);
  print_str(" from ");
  print_str(pWgetUrl);
  print_str("\r\n");

  wget();
}

uint8_t wget() {
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
      return 2;
    }
  }

  print_str(ERASE_LINE "Waiting for data ...");

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

  return 0;

abort:
#ifdef DIAGNOSTIC_FILE_CAPTURE
  fclose(fptrDiagnostics);
#endif
  if (pFileName) {
    fclose(fptr);
    remove(pTempFileName);
  }

  return 1;
}

char msxHubUrl[] = "https://msxhub.com/api/12345678/latest/get/12345678.zip  ";
char msxHubFileName[] = "12345678.zip  ";

char *str_append(char *dest, const char *src) {
  while (*src)
    *dest++ = *src++;

  *dest = 0;
  return dest;
}

char *str_append_upper(char *dest, const char *src) {
  while (*src)
    *dest++ = toupper(*src++);

  *dest = 0;
  return dest;
}
void subCommandMsxHub() {
  msxHubUrl[0] = 0;
  char *p = str_append(msxHubUrl, "https://msxhub.com/api/");
  p = str_append_upper(p, pMsxHubPackageName);
  p = str_append(p, "/latest/pages");

  pWgetUrl = msxHubUrl;
  pFileName = NULL;

  if (wget())
    return;

  const int numberOfPages = atoi(xmodemState.packetBuffer + 3);

  for (int i = 0; i < numberOfPages; i++) {
  }
  printf("\r\nINT----> %d\r\n", numberOfPages);

  // p = str_append_upper(p, pMsxHubPackageName);
  // p = str_append(p, ".zip");

  // msxHubFileName[0] = 0;
  // p = str_append_upper(msxHubFileName, pMsxHubPackageName);
  // str_append(p, ".zip");

  // pWgetUrl = msxHubUrl;
  // pFileName = msxHubFileName;
  // subCommandWGet();
}

void main(const int argc, const unsigned char **argv) {
  (void)argc;
  (void)argv;
  if (!fossil_link()) {
    extbio_fossil_install();

    if (!fossil_link()) {
      print_str("Fossil driver not found\r\n");
      exit(1);
    }
  }

  process_cli_arguments(argv, argc);

  uint16_t actual_baud_rate = fossil_set_baud(baud_rate);
  if (actual_baud_rate != baud_rate) {
    print_str("Actual Baud Rate: ");
    print_str(fossil_rate_to_string(actual_baud_rate));
    print_str("\r\n");
  }
  fossil_set_protocol(7); // 8N1
  fossil_init();

  if (subCommand == SUB_COMMAND_HANGUP) {
    delay(ONE_SECOND);
    fossil_rs_string("+++");
    delay(ONE_SECOND);
    fossil_rs_string("\r\nATH\r\n"); // hang up
    goto done;
  }

  fossil_rs_string("\r\nATE0\r\n"); // echo off

  if (subCommand == SUB_COMMAND_TIME_SYNC) {
    subCommandTimeSync();
    goto done;
  }

  if (subCommand == SUB_COMMAND_SET_TIMEZONE) {
    subCommandSetTimeZone();
    goto done;
  }

  if (subCommand == SUB_COMMAND_SET_WIFI) {
    subCommandSetWiFi();
    goto done;
  }

  if (subCommand == SUB_COMMAND_WGET) {
    subCommandWGet();
    goto done;
  }

  if (subCommand == SUB_COMMAND_MSXHUB) {
    subCommandMsxHub();
    goto done;
  }

done:
  fossil_deinit();
}
