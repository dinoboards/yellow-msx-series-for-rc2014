
#include <stdio.h>
#define x_printf     printf
#define print_string printf
#define print_hex(x) printf("%02x", x)

extern void print_uint16(const uint16_t n) __z88dk_fastcall;
