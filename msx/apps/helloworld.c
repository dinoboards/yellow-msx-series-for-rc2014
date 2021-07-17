
#include <stdio.h>

uint8_t fn(const uint8_t y) { return y + 10; }

void main(const int argc, const char *argv[]) {
  (void)argc;
  puts_cons("Hello World 3\r\n");
  puts_cons((char *)argv[0]);
  puts_cons((char *)argv[1]);
  printk(" %d\r\n", argc);
}
