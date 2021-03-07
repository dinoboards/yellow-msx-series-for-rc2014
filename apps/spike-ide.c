#define __Z88DK_R2L_CALLING_CONVENTION
#include "spike-ide.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ioBuf buf;

void main() {
  printf("spike-ide - ver: 0.4\r\n");

  printf("Retriving Identity info\r\n");

  const uint8_t d = cfProbe();

  if (d == 0) {
    printf("Compact Flash Module not detected\r\n");
    exit(0);
  }

  cfInit();

  cfReadIdentity(buf.data);

  IdeIdentity *x = (IdeIdentity *)buf.data;
  x->ModelNumber[39] = 0;
  x->SerialNumber[19] = 0;

  printf("General Configuration %04X\r\n", x->GeneralConfiguration);
  printf("Serial Number: %s\r\n", x->SerialNumber);
  printf("Model Number: %s\r\n", x->ModelNumber);
  printf("CurrentSectorsPerTrack %u\r\n", x->CurrentSectorsPerTrack);
  printf("CurrentSectorCapacity %lu\r\n", x->CurrentSectorCapacity);
  printf("Storage Capacity: %lu mega bytes\r\n", x->CurrentSectorCapacity * 512 / 1024 / 1024);
  printf("CurrentSectorCapacity Offset: %d\r\n", offsetof(struct _IdeIdentity, CurrentSectorCapacity));
  printf("ModelNumber Offset: %d\r\n", offsetof(struct _IdeIdentity, ModelNumber));

  const uint32_t lastSector = x->CurrentSectorCapacity - 1;

  buf.lba0 = lastSector;
  buf.lba1 = (lastSector >> 8) & 0xFF;
  buf.lba2 = (lastSector >> 16) & 0xFF;
  buf.lba3 = (lastSector >> 24) & 0xFF;

  printf("lba: %02X %02X %02X %02X", buf.lba0, buf.lba1, buf.lba2, buf.lba3);

  cfReadTest(&buf);

  printf("--------------\r\n");

  for (int i = 0; i < 128; i++) {
    if (i % 16 == 0)
      printf("\r\n");

    printf("%02X ", buf.data[i]);
  }

  for (int i = 0; i < 512; i++)
    buf.data[i] = i;

  cfWriteTest(&buf);
}
