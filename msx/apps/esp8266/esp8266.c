#include "esp8266.h"
#include "arguments.h"
#include "msxhub.h"
#include "print.h"
#include "wget.h"
#include <delay.h>
#include <extbio.h>
// #include <fossil.h>
#include <extbio/serial-helpers.h>
#include <extbio/serial.h>
#include <msxdos.h>
#include <stdio.h>
#include <string.h>
#include <system_vars.h>

unsigned char responseStr[MAX_RESPONSE_STRING_LEN + 1];
uint16_t      size;
uint8_t       port_number = 1;

void abortWithError(const char *pMessage) __z88dk_fastcall {
  if (pMessage) {
    print_str("\r\n");
    print_str(pMessage);
    print_str("\r\n");
  }
  exit(1);
}

bool serial_read_line(const bool withLogging) __z88dk_fastcall {
  withLogging;
  uint8_t        length  = 0;
  int16_t        timeout = get_future_time(from_ms(5000));
  unsigned char *pBuffer = responseStr;

  while (length < MAX_RESPONSE_STRING_LEN && !is_time_past(timeout)) {
    if (serial_get_rx_buffer_size(port_number) != 0) {
      timeout         = get_future_time(from_ms(5000));
      unsigned char t = 0;
      size            = 1;
      serial_read(port_number, &t, &size);
      if (size) {
        // if (withLogging)
        if (t >= 32 && t < 128) {
          printf("%c", t);

          *pBuffer++ = t;
          length++;
        } else
          printf("(%02X)", t);

        if (t == '\n')
          break;
      }
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
    if (lineReceived = serial_read_line(true)) {
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
  serial_purge_buffers(port_number);
  printf("sending time request\r\n");
  serial_write_bytes("\r\nAT+time?\r\n", 12);
  printf("Sent. waiting for response\r\n");
  if (serial_read_line(false)) {
    print_str("Error getting time\r\n");
    return;
  }

  printf("Read\r\n");

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
  serial_purge_buffers(port_number);
  serial_write_bytes("\r\nat+locale=", 12);
  serial_write_string(pNewTimeZone);
  serial_write_bytes("\r\n", 2);

  if (serial_read_line(false)) {
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
  serial_purge_buffers(port_number);
  serial_write_bytes("\r\nat+cwjap=", 11);
  serial_write_string(pNewSSID);
  serial_write_char(',');
  serial_write_string(pNewPassword);
  serial_write_bytes("\r\n", 2);

  read_until_ok_or_error();
}

void resetModem(void) {
  delay(ONE_SECOND);
  serial_write_bytes("+++", 3);
  delay(ONE_SECOND);
  serial_write_bytes("\r\nATH\r\n", 7); // hang up
  serial_read_line(false);
  serial_write_bytes("\r\nATE0\r\n", 8); // echo off
  serial_read_line(false);
}

uint8_t main(const int argc, const char *argv[]) {
  // TODO: find portnumber for supplied driver  name
  process_cli_arguments(argv, argc);

  serial_set_baudrate(port_number, baud_rate);
  serial_set_protocol(port_number, SERIAL_PARITY_NONE | SERIAL_STOPBITS_1 | SERIAL_BITS_8 | SERIAL_BREAK_OFF);

  if (subCommand == SUB_COMMAND_HANGUP) {
    delay(ONE_SECOND);
    serial_write_bytes("+++", 3);
    delay(ONE_SECOND);
    serial_write_bytes("\r\nATH\r\n", 7); // hang up
    goto done;
  }

  serial_write_bytes("\r\nATE0\r\n", 8); // echo off

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
  return 0;
}
