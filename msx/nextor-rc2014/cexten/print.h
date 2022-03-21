#ifndef __XPRINT
#define __XPRINT

int __printf(const char *fmt, ...);
int xprintf(const char *fmt, ...);

extern void print_init_screen0();

#define printf __printf

#endif
