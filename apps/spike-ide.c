#define __Z88DK_R2L_CALLING_CONVENTION
#include "spike-ide.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ioBuf   buf;
uint8_t result;


void main() {
  printf("spike-ide - ver: 0.4\r\n");

  printf("Retriving Identity info\r\n");

  const uint8_t d = cfProbe();

  if (d == 0) {
    printf("Compact Flash Module not detected\r\n");
    exit(0);
  }

  cfInit();

  result = cfReadIdentity(buf.data);
  if (result != 0) {
    printf("cfReadIdentity error %d\r\n", result);
    exit(0);
  }

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

  printf("lba: %02X %02X %02X %02X\r\n", buf.lba0, buf.lba1, buf.lba2, buf.lba3);

  result = cfReadTest(&buf);
  if (result != 0) {
    printf("cfReadTest error %d\r\n", result);
    exit(0);
  }
  printf("--------------\r\n");

  for (int i = 0; i < 128; i++) {
    if (i % 16 == 0)
      printf("\r\n");

    printf("%02X ", buf.data[i]);
  }

  for (int i = 0; i < 512; i++)
    buf.data[i] = buf.data[i] + 1;

  result = cfWriteTest(&buf);
  if (result != 0) {
    printf("cfWriteTest error %d\r\n", result);
    exit(0);
  }
}
