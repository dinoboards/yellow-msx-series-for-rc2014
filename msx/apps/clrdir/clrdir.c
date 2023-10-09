#include "arguments.h"
#include "fdisk2.h"
#include <fat.h>
#include <io.h>
#include <msxdos.h>
#include <stdio.h>
#include <system_vars.h>

#define SLOT_3_3    0x8F
#define deviceIndex 1
#define lunIndex    1

uint16_t dumpSector(const uint32_t number) {
  uint8_t error;

  clearSectorBuffer();

  printf("Sector %d\r\n", number);
  if ((error = readSectorFromDevice(SLOT_3_3, deviceIndex, lunIndex, number)) != 0) {
    printf("readSectorFromDevice: %d\n", error);
    return error;
  }

  for (int i = 0; i < 32; i++) {
    printf("%02X ", sectorBuffer[i]);
    if (i % 16 == 15)
      printf("\n");
  }
  return 0;
}

uint16_t testReadWrite(void) {
  const fatBootSector *sector = (fatBootSector *)sectorBuffer;

  // msxdosLunInfo lunInfo;
  // memset(&lunInfo, 0, sizeof(msxdosLunInfo));

  uint16_t error;
  int16_t  i;

  // error = msxdosDrvLunInfo(SLOT_3_3, deviceIndex, lunIndex, &lunInfo);

  // printf("msxdosDrvLunInfo: %d\n", error);
  // printf("mediumType: %d, ", lunInfo.mediumType);
  // printf("sectorSize: %d, ", lunInfo.sectorSize);
  // printf("sectorCount: %d, ", lunInfo.sectorCount);
  // printf("flags: %d, ", lunInfo.flags);
  // printf("cylinders: %d, ", lunInfo.cylinders);
  // printf("heads: %d, ", lunInfo.heads);
  // printf("sectorsPerTrack: %d, ", lunInfo.sectorsPerTrack);
  // printf("suitableForPartitioning: %d, ", lunInfo.suitableForPartitioning);
  // printf("number: %d\r\n", lunInfo.number);

  clearSectorBuffer();
  uint8_t marker = (uint8_t)JIFFY;
  printf("Marking sectors %d\n\r", marker);
  for (i = 6; i <= 8; i++) {
    sectorBuffer[3] = marker;
    sectorBuffer[4] = i + (marker & 0xF0);
    printf(".");
    error = writeSectorToDevice(SLOT_3_3, deviceIndex, lunIndex, (uint32_t)i);
    printf("writeSectorToDevice: %d\n", error);
    if (error) {
      const char errorMessage[100];
      memset(errorMessage, 0, sizeof(errorMessage));
      msxdosExplain(error, errorMessage);
      printf(errorMessage);
    }
  }

  memset(sectorBuffer, 0xFF, sizeof(sectorBuffer));

  for (i = 6; i <= 8; i++)
    if ((error = dumpSector(i)) != 0)
      return error;

  return 0;
}

uint16_t captureSectorsToFile(void) {
  uint16_t error;
  int      iFile = 0;

  iFile = Open("sector.hex", IO_CREAT);

  for (int i = 0; i < 2; i++) {
    printf(">");
    if ((error = dumpSector(i)) != 0)
      return error;
    printf(".");
    Write(iFile, sectorBuffer, 512);
  }

  Close(iFile);
  return 0;
}

uint8_t main(const int argc, const unsigned char **argv) {
  process_cli_arguments(argc, argv);

  driveLetterInfo info;
  memset(&info, 0, sizeof(driveLetterInfo));

  uint8_t result = msxdosGetDriveLetterInfo(drive_letter - 'A', &info);

  msxdosLunInfo lunInfo;
  memset(&lunInfo, 0, sizeof(msxdosLunInfo));

  result = msxdosDrvLunInfo(info.phyical.slot, info.phyical.index, info.phyical.lun, &lunInfo);

  if (!(lunInfo.flags & LUN_INFO_FLAG_FLOPPY)) {
    printf("Only floppy disk are supported. Unable to proceed.\r\n");
    return 255;
  }

  if (lunInfo.sectorCount != 2880) {
    printf(
        "Unexpceted sector count for device. Expected 2880, got %ld\r\nOnly 1.44Mb floppies are supported. Unable to proceed.\r\n",
        lunInfo.sectorCount);
    return 255;
  }

  printf("WARNING, ALL DATA ON\r\nDRIVE %c: WILL BE LOST!\r\nProceed with Format (Y/N)? ", drive_letter);

  char c = getchar();
  if (c != 'Y' && c != 'y') {
    printf("\r\n");
    return 255;
  }

  printf("\r\n");

  result = createFat12FileSystem(info.phyical.slot, info.phyical.index, info.phyical.lun);
  if (result != 0) {
    const char errorMessage[100];
    memset(errorMessage, 0, sizeof(errorMessage));
    msxdosExplain(result, errorMessage);

    printf("\r\nFailed %d: %s\r\n", result, errorMessage);
    return 255;
  }

  printf("\r\nCompleted.\r\n");
  return 0;
}
