
#include "print.h"
#include <stdio.h>

void print_str(const char *p) __z88dk_fastcall {
  while (*p)
    fputc_cons(*p++);
}
