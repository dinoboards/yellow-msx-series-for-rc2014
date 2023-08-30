
#include "msxdos.h"
#include <stdio.h>
#include <system_vars.h>

uint8_t fn(const uint8_t y) { return y + 10; }

void xx(uint8_t *a) { *a = *a + 1; }

uint8_t test(uint8_t aa) {
  uint8_t a;
  // uint8_t b;
  // uint8_t c;
  // uint8_t d;
  // uint8_t e;
  a = 1 + aa;
  // b = 2+aa;
  // c = 3+aa;
  // d = 4+aa;
  // e = 5 + aa;

  for (uint8_t x = 1; x < 100; x++) {
    xx(&a);
    // xx(&b);
    // xx(&c);
    printf("%d", a);
  }

  return a;
}
void main(const int argc, const char *argv[]) {
  (void)argc;
  printk("     Hello World 3: %d, %d, %d\r\n", JIFFY, CSRX, CSRY);
  printk("     Hello World 4: %d, %d, %d\r\n", JIFFY, CSRX, CSRY);

  const uint8_t rowAbove = CSRY - 1;
  msxbiosPosit(1, rowAbove);
  printk("%s %d\r\n", argv[1], argc);

  test(1);
}
