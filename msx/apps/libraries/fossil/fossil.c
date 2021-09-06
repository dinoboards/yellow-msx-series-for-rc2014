
#include "fossil.h"

void fossil_rs_string(const char *pString) {
  while (*pString)
    fossil_rs_out(*pString++);
}

/***
 * consume characters on input stream, until no characters received for a short period
 *
 */
void fossil_rs_flush() {
  unsigned int i = 65000;

  while (i != 0) {
    i--;

    while (fossil_rs_in_stat() != 0) {
      fossil_rs_in();
      i = 65000;
    }
  }
}
