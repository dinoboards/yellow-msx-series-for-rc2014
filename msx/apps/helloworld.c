
#include "msxdos.h"
#include <msx.h>
#include <stdio.h>

uint8_t fn(const uint8_t y) { return y + 10; }

void main(const int argc, const char *argv[]) {
  (void)argc;
  printk("     Hello World 1: %d, %d, %d\r\n", JIFFY, CSRX, CSRY);
  printk("     Hello World 2: %d, %d, %d\r\n", JIFFY, CSRX, CSRY);
  msxbiosPosit(1, CSRY - 1);
  // puts_cons((char *)argv[0]);
  // puts_cons((char *)argv[1]);
  printk("%s %d\r\n", argv[1], argc);
}
