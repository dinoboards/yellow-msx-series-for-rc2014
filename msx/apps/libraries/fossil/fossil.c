
#include "fossil.h"
#include "delay.h"

void fossil_rs_string(const char *pString) {
  while (*pString)
    fossil_rs_out(*pString++);
}

/***
 * consume characters on input stream, until no characters received for a short period
 *
 */
void fossil_rs_flush(void) {
  int16_t timeout = get_future_time(from_ms(1500));

  while (!is_time_past(timeout)) {
    while (fossil_rs_in_stat() != 0) {
      fossil_rs_in();
      timeout = get_future_time(from_ms(1500));
    }
  }
}
