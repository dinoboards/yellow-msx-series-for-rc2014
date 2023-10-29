
#include "msxdos.h"
#include "xstdio.h"
#include <extbio.h>
#include <stdio.h>

void main(void) {
  void *p = extbio_fossil_install(FOSSIL_FOR_SIO2);

  printf("Fossil Driver Activated at address: %p\r\n", p);
}
