
#include "fdisk2.h"
#include <fat.h>
#include <msxdos.h>
#include <partition.h>
#include <stdio.h>
#include <string.h>

uint8_t sectorBuffer[512];

static int getNewSerialNumber(void) { return 0; }

uint16_t deviceSectorRead(uint8_t driverSlot, uint8_t deviceIndex, uint8_t lunIndex, uint32_t sectorNumber) {
  return msxdosDevRead(driverSlot, deviceIndex, lunIndex, sectorNumber, 1, sectorBuffer);
}

uint16_t deviceSectorWrite(uint8_t driverSlot, uint8_t deviceIndex, uint8_t lunIndex, uint32_t sectorNumber) {
  return msxdosDevWrite(driverSlot, deviceIndex, lunIndex, sectorNumber, 1, sectorBuffer);
}

void clearSectorBuffer(void) { memset(sectorBuffer, 0, sizeof(sectorBuffer)); }

static void createFat12BootSector(void) {
  fatBootSector *sector = (fatBootSector *)sectorBuffer;

  clearSectorBuffer();

  sector->jumpInstruction[0] = 0xEB;
  sector->jumpInstruction[1] = 0xFE;
  sector->jumpInstruction[2] = 0x90;
  strcpy(sector->oemNameString, "MSXDOS");
  sector->sectorSize           = 512;
  sector->sectorsPerCluster    = 1;
  sector->reservedSectors      = 1;
  sector->numberOfFats         = FAT_COPIES;
  sector->rootDirectoryEntries = 224;
  sector->smallSectorCount     = 2880;
  sector->mediaId              = 0xF0;
  sector->sectorsPerFat        = 9;
  sector->sectorsPerTrack      = 18;
  sector->numberOfHeads        = 2;
  strcpy(sector->params.standard.volumeLabelString, "XYZ "); // it is same for DOS 2.20 format
  sector->params.standard.serialNumber        = 1234;        // it is same for DOS 2.20 format
  sector->mbrSignature                        = 0xAA55;
  sector->params.DOS220.z80JumpInstruction[0] = 0x18; // JUMP 0x1E
  sector->params.DOS220.z80JumpInstruction[1] = 0x1E; // MSXDOS as JUMP 0x10
  strcpy(sector->params.DOS220.volIdString, "VOL_ID");
  strcpy(sector->params.DOS220.fatTypeString, "FAT12   ");
  memcpy(&(sector->params.DOS220.z80BootCode), sectorBootCode, (uint16_t)0xC090 - (uint16_t)0xC03E);
}

extern uint8_t superBuffer[4096];
#define SUPER_BUFFER_SEC_COUNT (4096 / 512)

#define min(a, b) ((a) < (b) ? (a) : (b))

uint8_t createFat12FileSystem(uint8_t driverSlot, uint8_t deviceIndex, uint8_t lunIndex) {
  const fatBootSector *sector = (fatBootSector *)sectorBuffer;

  uint8_t  error;
  uint32_t sectorNumber;
  uint16_t zeroSectorsToWrite;
  int16_t  i;

  createFat12BootSector();
  const uint16_t sectorsPerFat        = sector->sectorsPerFat;
  const uint16_t rootDirectoryEntries = sector->rootDirectoryEntries;

  if ((error = writeSectorToDevice(driverSlot, deviceIndex, lunIndex, 0)) != 0)
    return error;

  clearSectorBuffer();
  memset(superBuffer, 0, sizeof(superBuffer));

  zeroSectorsToWrite = (sectorsPerFat * FAT_COPIES) + rootDirectoryEntries - 1;
  sectorNumber       = 1;

  i = zeroSectorsToWrite;
  while (i > 0) {
    printf(".");
    const int16_t remaining = min(i, SUPER_BUFFER_SEC_COUNT);
    if ((error = msxdosDevWrite(driverSlot, deviceIndex, lunIndex, sectorNumber++, remaining, superBuffer)) != 0)
      return error;

    i -= remaining;
  }

  // //* First sector of each FAT

  sectorBuffer[0] = 0xF0;
  sectorBuffer[1] = 0xFF;
  sectorBuffer[2] = 0xFF;

  if ((error = writeSectorToDevice(driverSlot, deviceIndex, lunIndex, 1)) != 0)
    return error;

  if ((error = writeSectorToDevice(driverSlot, deviceIndex, lunIndex, sectorsPerFat + 1)) != 0)
    return error;

  return 0;
}
