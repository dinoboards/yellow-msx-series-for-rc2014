#include "utils.h"

BUFFER_ADDR static unsigned char buffer[30];

const unsigned char *uint32_to_string(uint32_t value) __z88dk_fastcall {
  unsigned char *ptr = buffer;
  uint32_t       temp;

  if (value == 0) {
    *ptr++ = '0';
    *ptr++ = '\0';
    return buffer;
  }

  for (temp = value; temp > 0; temp /= 10, ptr++)
    ;
  *ptr = '\0';

  for (temp = value; temp > 0; temp /= 10)
    *--ptr = temp % 10 + '0';

  return buffer;
}
