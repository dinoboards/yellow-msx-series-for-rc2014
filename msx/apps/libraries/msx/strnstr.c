#include "strnstr.h"
#include <string.h>

/**
 * @brief locates the first occurrence of the null-terminated string \c needle in the string \c haystack,
 * where not more than \c len characters are searched. Characters that appear after a \0 character are
 * not searched.
 *
 * @param haystack
 * @param needle
 * @param len
 * @return char*
 */
char *strnstr(const char *haystack, const char *needle, size_t len) __sdcccall(1) {
  int    i;
  size_t needle_len;

  if (0 == (needle_len = strnlen(needle, len)))
    return (char *)haystack;

  for (i = 0; i <= (int)(len - needle_len); i++) {
    if ((haystack[0] == needle[0]) && (0 == strncmp(haystack, needle, needle_len)))
      return (char *)haystack;

    haystack++;
  }
  return NULL;
}
