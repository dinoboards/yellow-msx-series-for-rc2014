#define __Z88DK_R2L_CALLING_CONVENTION
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spike-ide.h"

ioBuf buf;


void main() {
  printf("spike-ide - ver: 0.4\r\n");

  printf("Retriving Identity info\r\n");

  cfInit();

  cfReadIdentity(buf.data);

  // buf.lba0 = buf.lba1 = buf.lba2 = buf.lba3 = 0;
  // cfReadTest(&buf);

  printf("--------------\r\n");

  // for(int i = 0; i < 128; i++) {
  //   if (i % 16 == 0)
  //     printf("\r\n");

  //   printf("%02X ", buf.data[i]);
  // }

  IdeIdentity* x = (IdeIdentity*)buf.data;
  x->ModelNumber[39] = 0;
  x->SerialNumber[19] = 0;

  printf("Serial Number: %s\r\n", x->SerialNumber);
  printf("Model Number: %s\r\n", x->ModelNumber);
  printf("CurrentSectorsPerTrack %u\r\n", x->CurrentSectorsPerTrack);
  printf("CurrentSectorCapacity %lu\r\n", x->CurrentSectorCapacity);
  printf("Storage Capacity: %lu mega bytes\r\n", x->CurrentSectorCapacity * 512 / 1024 / 1024);
}
