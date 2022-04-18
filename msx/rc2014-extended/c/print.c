
#include "print.h"

#if 0

#include <stdarg.h>
#include <stdio.h>

#define MAX_PRINT_STRING_LEN 200

int __printf(const char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);

  char buf[MAX_PRINT_STRING_LEN];
  int  len = vsnprintf(buf, MAX_PRINT_STRING_LEN - 1, (char *)fmt, arg);

  print_string(buf);

  va_end(arg);
  return len;
}

int xprintf(const char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);

  char buf[MAX_PRINT_STRING_LEN];
  int  len = vsnprintf(buf, MAX_PRINT_STRING_LEN - 1, (char *)fmt, arg);

  x_print_string(buf);

  va_end(arg);
  return len;
}

#endif
