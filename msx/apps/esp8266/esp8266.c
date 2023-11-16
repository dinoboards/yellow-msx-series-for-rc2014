#include "esp8266.h"
#include "arguments.h"
#include "msxhub.h"
#include "print.h"
#include "wget.h"
#include <delay.h>
#include <extbio.h>
#include <fossil.h>
#include <msxdos.h>
#include <stdio.h>
#include <string.h>
#include <system_vars.h>

unsigned char responseStr[MAX_RESPONSE_STRING_LEN + 1];

void abortWithError(const char *pMessage) __z88dk_fastcall {
  if (pMessage) {
    print_str("\r\n");
    print_str(pMessage);
    print_str("\r\n");
  }
  exit(1);
}

void fossil_rs_flush_with_log(void) {
  int16_t timeout = get_future_time(from_ms(5000));

  while (!is_time_past(timeout)) {
    while (fossil_rs_in_stat() != 0) {
      unsigned char c = fossil_rs_in();
      printf("%c", c);
      timeout = get_future_time(from_ms(5000));
    }
  }
}

bool fossil_rs_read_line(const bool withLogging) __z88dk_fastcall {
  uint8_t        length  = 0;
  int16_t        timeout = get_future_time(from_ms(5000));
  unsigned char *pBuffer = responseStr;

  while (length < MAX_RESPONSE_STRING_LEN && !is_time_past(timeout)) {
    if (fossil_rs_in_stat() != 0) {
      timeout               = get_future_time(from_ms(5000));
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

bool read_until_ok_or_error(void) {
  int16_t timeout      = get_future_time(from_ms(2000));
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

void subCommandTimeSync(void) {
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

  const int year  = atoi(responseStr);
  const int month = atoi(responseStr + 5);
  const int day   = atoi(responseStr + 8);
  const int hour  = atoi(responseStr + 11);
  const int min   = atoi(responseStr + 14);
  const int sec   = atoi(responseStr + 17);

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

void subCommandSetTimeZone(void) {
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

void subCommandSetWiFi(void) {
  fossil_rs_flush();
  fossil_rs_string("\r\nat+cwjap=");
  fossil_rs_string(pNewSSID);
  fossil_rs_string(",");
  fossil_rs_string(pNewPassword);
  fossil_rs_string("\r\n");

  read_until_ok_or_error();
}

void resetModem(void) {
  delay(ONE_SECOND);
  fossil_rs_string("+++");
  delay(ONE_SECOND);
  fossil_rs_string("\r\nATH\r\n"); // hang up
  fossil_rs_read_line(false);
  fossil_rs_string("\r\nATE0\r\n"); // echo off
  fossil_rs_read_line(false);
}

uint8_t main(const int argc, const char *argv[]) {
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

  return 0;
}
