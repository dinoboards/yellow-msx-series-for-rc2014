
#include "msxdos.h"
#include <stdio.h>
#include <system_vars.h>

uint8_t fn(const uint8_t y) { return y + 10; }

void main(const int argc, const char *argv[]) {
  (void)argc;
  printk("     Hello World 3: %d, %d, %d\r\n", JIFFY, CSRX, CSRY);
  printk("     Hello World 4: %d, %d, %d\r\n", JIFFY, CSRX, CSRY);

  const uint8_t rowAbove = CSRY - 1;
  msxbiosPosit(1, rowAbove);
  printk("%s %d\r\n", argv[1], argc);
}
