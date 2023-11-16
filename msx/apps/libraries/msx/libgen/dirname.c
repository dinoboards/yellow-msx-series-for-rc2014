#include "../libgen.h"
#include <string.h>

char *dirname(char *const path) __z88dk_fastcall {
  char *last_slash = strrchr(path, '\\');
  if (last_slash) {
    last_slash++;
    *last_slash = '\0';
    return path;
  }

  if (strchr(path, ':')) {
    path[2] = '\0';
    return path;
  }

  path[0] = '\0';
  return path;
}
