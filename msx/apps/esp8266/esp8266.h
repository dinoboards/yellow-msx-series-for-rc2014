#ifndef __ESP8266
#define __ESP8266

#include <stdbool.h>

#define ERASE_LINE              "\x1B\x6C\r"
#define CTRLZ                   0x1A
#define ONE_SECOND              (VDP_FREQUENCY * 1.1)
#define MAX_RESPONSE_STRING_LEN 128

extern unsigned char responseStr[MAX_RESPONSE_STRING_LEN + 1];

extern void fossil_rs_flush_with_log();
extern bool fossil_rs_read_line(const bool withLogging) __z88dk_fastcall;

extern void resetModem();

extern void abortWithError(const char *pMessage) __z88dk_fastcall;

#endif
