#ifndef __XPRINT
#define __XPRINT

#include <stdlib.h>

extern void print_hex(const char c) __z88dk_fastcall;
extern void print_string(const char *p) __z88dk_fastcall;
extern void x_print_string(const char *p) __z88dk_fastcall;

int __printf(const char *fmt, ...);
int xprintf(const char *fmt, ...);

extern void print_init_screen0(const uint16_t p) __z88dk_fastcall;

#define printf __printf

#define yprintf(p, fmt, ...)                                                                                                \
  {                                                                                                                         \
    print_init_screen0(p);                                                                                                  \
    xprintf(fmt, ##__VA_ARGS__);                                                                                            \
  }

#define x_printf(...) /* disabled */

#endif
