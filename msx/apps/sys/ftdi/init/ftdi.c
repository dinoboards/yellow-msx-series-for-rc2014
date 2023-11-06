#include <stdio.h>
#include <sysload/sysload.h>

extern const char *helloworld;

#define relocated(x) ((const char *)((uint16_t)(&x) + (uint16_t)(my_header)))

uint16_t init(installed_sys_t *my_header) __z88dk_fastcall {
  printf("my_header: %p\r\n", my_header);
  printf("string: %s", relocated(helloworld));
  return 0;
}
