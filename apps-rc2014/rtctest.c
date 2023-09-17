#include "cpm.h"
#include "rp5c01.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inline uint8_t bcdToDecimal(uint8_t hex) { return ((hex & 0xF0) >> 4) * 10 + (hex & 0x0F); }

static void convert(rtcDateTime *datetime) __z88dk_fastcall {
  datetime->second = bcdToDecimal(datetime->second);
  datetime->minute = bcdToDecimal(datetime->minute);
  datetime->hour   = bcdToDecimal(datetime->hour);
  datetime->day    = bcdToDecimal(datetime->day);
  datetime->month  = bcdToDecimal(datetime->month);
  datetime->year   = bcdToDecimal(datetime->year);
}

rtcDateTime datetime;
char        line[10];

uint8_t stringToBcd() {
  const uint8_t firstNibble  = (line[0] - '0') & 0xF;
  const uint8_t secondNibble = (line[1] - '0') & 0xF;

  return (firstNibble << 4) + secondNibble;
}

uint8_t readSetting() {
  line[0] = 0;   /* Ensure empty line if no input delivered */
  line[8] = 255; //~'\0';  /* Ensure no false-null at end of buffer */
  line[9] = 0;   //~'\0';

  while (true) {
    fgets(line, 9, stdin);

    if (strlen(line) != 3) { // includes line terminator
      printf("\r\nBad value. Please enter 2 digits only.\r\n");
      continue;
    }

    break;
  }

  return stringToBcd();
}

void main() {
  printf("Test application for RC5C01 RTC chip\r\n");

  const uint8_t detected = rp5c01Detect();

  if (detected)
    printf("RP5C01 detected\r\n");
  else {
    printf("RP5C01 not detected\r\n");
    exit(1);
  }

  const uint8_t errorCode = rp5c01SetByte(0, 0x48);
  if (errorCode) {
    printf("rp5c01SetByte return error code %02X\r\n", errorCode);
    exit(1);
  }
  const uint16_t data = rp5c01GetByte(0);

  if (data != 0x48)
    printf("NVRAM failure - wrote 0x48, got %02X returned (error code: %02X).\r\n", data & 255, data > 8);
  else
    printf("NVRAM success for index 1\r\n");

  rp5c01TestMode(0);           // Bit 0 to bit 3 can be set to place chip into test modes
  rp5c01SetHourMode(HR_MD_24); // 24 hour clock
  rp5c01SetMode(MD_TIME);      // Enable timer

  printf("Showing current time. Type S to set time. Press any key to exit\r\n");

  // hide terminal cursor
  printf("\x1b[?25l");

  char t;

loop:
  while ((t = cRawIo()) == 0) {
    rp5c01GetTime(&datetime);
    convert(&datetime);

    printf("%02d-%02d-%02d %02d:%02d:%02d\x1b[0G", datetime.year, datetime.month, datetime.day, datetime.hour, datetime.minute,
           datetime.second);
  }

  if (t == 'S' || t == 's') {
    printf("\x1b[?25h");
    printf("\r\nSet time.....\r\n");

    printf("Supply the 2 digits values  year, month, day, hour, minute and second\r\n");

    printf("Year: ");
    datetime.year = readSetting();

    printf("\r\nMonth: ");
    datetime.month = readSetting();

    printf("\r\nDay: ");
    datetime.day = readSetting();

    printf("\r\nHour: ");
    datetime.hour = readSetting();

    printf("\r\nMinute: ");
    datetime.minute = readSetting();

    printf("\r\nSeconds: ");
    datetime.second = readSetting();

    // printf("\r\n%02x-%02x-%02x %02x:%02x:%02x\r\n\r\n", datetime.year, datetime.month, datetime.day, datetime.hour,
    // datetime.minute, datetime.second);

    rp5c01SetMode(0); // Disable timer (and alarm)
    rp5c01SetTime(&datetime);
    rp5c01SetMode(MD_TIME); // Enable timer

    printf("\x1b[?25l");
    goto loop;
  }

  // show terminal cursor
  printf("\x1b[?25h");
}
