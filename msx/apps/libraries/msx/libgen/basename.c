#include "../libgen.h"
#include <string.h>

const char *basename(const char *const path) __z88dk_fastcall {
  const char *last_slash = strrchr(path, '\\');
  if (last_slash)
    return last_slash + 1;

  const char *colon = strchr(path, ':');
  if (colon)
    return colon + 1;

  return path;
}
