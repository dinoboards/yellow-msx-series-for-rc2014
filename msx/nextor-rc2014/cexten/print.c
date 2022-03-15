#include "print.h"
#include <stdio.h>
#include <stdarg.h>

extern void print_string(const char* p) __z88dk_fastcall;

#define MAX_PRINT_STRING_LEN 200

int xprintf(const char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);

  char buf[MAX_PRINT_STRING_LEN];
  int len = vsnprintf(buf, MAX_PRINT_STRING_LEN - 1, (char *)fmt, arg);

  print_string(buf);

  va_end(arg);
  return len;
}
