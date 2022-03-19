#ifndef __XPRINT
#define __XPRINT

int xprintf(const char *fmt, ...);

#define printf xprintf

#endif
