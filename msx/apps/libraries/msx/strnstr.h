#ifndef _STRNSTR_H_
#define _STRNSTR_H_

#include <stddef.h>

extern char *strnstr(const char *haystack, const char *needle, size_t len) __sdcccall(1);

#endif
