
#include "fusion_ext.h"
#include "io.h"

#include <stdbool.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

int fusion_fprintf(const int file_handle, const char *const fmt, ...) {
  char buffer[256];

  memset(buffer, 0, sizeof(buffer));

  va_list args;
  va_start(args, fmt);
  const int result = vsnprintf(buffer, 256, fmt, args);
  va_end(args);
  Write(file_handle, buffer, strlen(buffer));

  return result;
}
