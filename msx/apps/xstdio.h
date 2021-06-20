#ifndef __XSTDIO
#define __XSTDIO

// Work around compiler error that seems to scramble the strings for printf
extern void xprintf(const char *msg, ...);

#endif
