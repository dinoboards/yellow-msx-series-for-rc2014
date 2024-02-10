#ifndef __ESP8266
#define __ESP8266

#include <stdbool.h>
#include <stdint.h>

#define ERASE_LINE              "\x1B\x6C\r"
#define CTRLZ                   0x1A
#define ONE_SECOND              (VDP_FREQUENCY * 1.1)
#define MAX_RESPONSE_STRING_LEN 128

extern unsigned char responseStr[MAX_RESPONSE_STRING_LEN + 1];

extern void fossil_rs_flush_with_log(void);
extern bool serial_read_line(void);

extern void resetModem(void);

extern void abortWithError(const char *pMessage) __z88dk_fastcall;

#endif
