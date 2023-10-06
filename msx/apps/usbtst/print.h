

/* must include stdbool before stdio, as stdio include msx.h which will define its own bool if none defined */
#include <stdbool.h>

#include <stdio.h>
#define print_string printf
#define print_hex(x) printf("%02x", x)

extern void print_uint16(const uint16_t n) __z88dk_fastcall;
