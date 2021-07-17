
#include <stdio.h>

uint8_t fn(const uint8_t y) { return y + 10; }

void main() {
  uint16_t x = fn(23);

  if (x > 10)
    // printk("Hello World %d\r\n", x);
    puts_cons("Hello World\r\n");
}
