
#include "extbio.h"
#include "msxdos.h"
#include "xstdio.h"
#include <stdio.h>

void main() {
  void *p = extbio_fossil_install();

  printf("Fossil Driver Activated at address: %p\r\n", p);
}
