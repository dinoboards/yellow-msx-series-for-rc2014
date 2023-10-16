#ifndef __FDISK
#define __FDISK

#include "msxdos.h"
#include <stdbool.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

void    printSize(uint32_t sizeInK);
uint8_t getRemainingBy1024String(uint32_t value, char *destination);

#endif
