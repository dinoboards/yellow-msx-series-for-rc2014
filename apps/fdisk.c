#define __Z88DK_R2L_CALLING_CONVENTION
#include "fdisk.h"
#include "msxdos.h"
#include "partition.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CAPUTED ENUMERATED DRIVER/DEVICE/LUN INFO
msxdosDriverInfo drivers[MAX_INSTALLED_DRIVERS];
deviceInfo       devices[MAX_DEVICES_PER_DRIVER];
msxddosLunInfo   luns[MAX_LUNS_PER_DEVICE];

msxdosDriverInfo *pSelectedDriverInfo;
deviceInfo *      pSelectedDeviceInfo;

struct _partitionState partitionState;

uint8_t retrieveAllDriverInfos() {
  uint8_t           result = 0;
  msxdosDriverInfo *pDriverInfo = drivers;
  uint8_t           count = 1;
  char              slotDescription[4];

  printf("Installed Drivers:\r\n");
  while (count <= MAX_INSTALLED_DRIVERS && (result = msxDosGdrvr(count, pDriverInfo)) == 0) {
    terminateRightPaddedString(pDriverInfo->driverName, DRIVER_NAME_LENGTH);
    composeSlotString(pDriverInfo->slot, slotDescription);
    pDriverInfo->number = count;
    printf("  %d: %s | %s\r\n", count, slotDescription, pDriverInfo->driverName);
    count++;
    pDriverInfo++;
  }

  return count - 1;
}

void retrieveAllDeviceInfo() {

  uint8_t     count = 1;
  uint16_t    result;
  deviceInfo *pDeviceInfo = devices;

  printf("\r\nActive Devices for (%d):\r\n", pSelectedDriverInfo->number);

  while (count <= MAX_DEVICES_PER_DRIVER && (result = msxdosDrvDevLogicalUnitCount(pSelectedDriverInfo->slot, count, (msxdosDeviceBasicInfo *)pDeviceInfo) == 0)) {
    result = msxdosDrvDevGetName(pSelectedDriverInfo->slot, count, pDeviceInfo->deviceName);
    pDeviceInfo->number = count;

    if (result == 0)
      terminateRightPaddedString(pDeviceInfo->deviceName, MAX_INFO_LENGTH);
    else
      sprintf(pDeviceInfo->deviceName, "Unnamed device, ID=%d", pSelectedDriverInfo->number);

    printf("\x1BK%d. %s\r\n\r\n", count, pDeviceInfo->deviceName);

    count++;
    pDeviceInfo++;
  }
}

uint8_t retrieveLunInfors() {
  msxddosLunInfo *pLunInfo = luns;
  uint8_t         count = 1;
  uint16_t        result = 0;

  while (count <= MAX_LUNS_PER_DEVICE) {
    result = msxdosDrvLunInfo(pSelectedDriverInfo->slot, pSelectedDeviceInfo->number, count, pLunInfo);

    if (result == 0) {
      pLunInfo->suitableForPartitioning =
          (result == 0) && (pLunInfo->mediumType == BLOCK_DEVICE) && (pLunInfo->sectorSize == 512) && (pLunInfo->sectorCount >= MIN_DEVICE_SIZE_IN_K * 2) && ((pLunInfo->flags & (READ_ONLY_LUN | FLOPPY_DISK_LUN)) == 0);

      if (pLunInfo->sectorsPerTrack == 0 || pLunInfo->sectorsPerTrack > EXTRA_PARTITION_SECTORS)
        pLunInfo->sectorsPerTrack = EXTRA_PARTITION_SECTORS;

      pLunInfo->number = count;

      printf("\x1B  %d: Capacity: ", count);
      printSize(pLunInfo->sectorCount / 2);
      if (pLunInfo->suitableForPartitioning)
        printf(" [SELECTABLE]\r\n");
      else
        printf(" [NOT SELECTABLE]\r\n");
    }

    count++;
    pLunInfo++;
  }

  return count - 1;
}

char input[4];

void main() {
  printf("FDISK Utility\r\n");

  uint8_t installedDriverCount = retrieveAllDriverInfos();

  printf("Enter driver number: ");
  fgets(input, sizeof(input), stdin);
  uint8_t selectedDriverNumber = atoi(input);
  pSelectedDriverInfo = &drivers[selectedDriverNumber - 1];

  retrieveAllDeviceInfo();

  printf("Enter device number: ");
  fgets(input, sizeof(input), stdin);
  uint8_t selectedDeviceNumber = atoi(input);
  pSelectedDeviceInfo = &devices[selectedDeviceNumber - 1];

  retrieveLunInfors();
  // printf("Enter logical unit number: ");
  // fgets(input, sizeof(input), stdin);
  // uint8_t selectedLun = atoi(input);

  // msxddosLunInfo* pSelectedLunInfo = &luns[selectedLun];

  // preparePartitionAnalysis(pSelectedLunInfo);
}

void recalculateAutoPartitionSize(bool setToAllSpaceAvailable) {
  ulong maxAbsolutePartitionSizeInK = (partitionState.unpartitionnedSpaceInSectors - EXTRA_PARTITION_SECTORS) / 2;

  if (setToAllSpaceAvailable)
    partitionState.autoSizeInK = maxAbsolutePartitionSizeInK;

  if (partitionState.autoSizeInK > MAX_FAT16_PARTITION_SIZE_IN_K) {
    partitionState.autoSizeInK = MAX_FAT16_PARTITION_SIZE_IN_K;
  } else if (!setToAllSpaceAvailable && partitionState.autoSizeInK > maxAbsolutePartitionSizeInK) {
    partitionState.autoSizeInK = maxAbsolutePartitionSizeInK;
  }

  if (partitionState.autoSizeInK < MIN_PARTITION_SIZE_IN_K) {
    partitionState.autoSizeInK = MIN_PARTITION_SIZE_IN_K;
  } else if (partitionState.autoSizeInK > maxAbsolutePartitionSizeInK) {
    partitionState.autoSizeInK = maxAbsolutePartitionSizeInK;
  }
}

void preparePartitionAnalysis(msxddosLunInfo *pSelectedLunInfo) {
  partitionState.pLunInfo = pSelectedLunInfo;
  partitionState.canCreate = (pSelectedLunInfo->sectorCount >= (MIN_DEVICE_SIZE_FOR_PARTITIONS_IN_K * 2));
  partitionState.canDoDirectFormat = (pSelectedLunInfo->sectorCount <= MAX_DEVICE_SIZE_FOR_DIRECT_FORMAT_IN_K * 2);
  partitionState.unpartitionnedSpaceInSectors = pSelectedLunInfo->sectorCount;
  recalculateAutoPartitionSize(true);
}

void terminateRightPaddedString(char *string, uint8_t length) {
  char *pointer = string + length - 1;
  while (*pointer == ' ' && length > 0) {
    pointer--;
    length--;
  }
  pointer[1] = '\0';
}

void composeSlotString(uint8_t slot, char *destination) {
  if ((slot & 0x80) == 0) {
    destination[0] = slot + '0';
    destination[1] = '\0';
  } else {
    destination[0] = (slot & 3) + '0';
    destination[1] = '-';
    destination[2] = ((slot >> 2) & 3) + '0';
    destination[3] = '\0';
  }
}

void printSize(uint32_t sizeInK) {
  char     buf[3];
  uint32_t dividedSize;

  if (sizeInK < (uint32_t)(10 * 1024)) {
    printf("%dK", sizeInK);
    return;
  }

  dividedSize = sizeInK >> 10;
  if (dividedSize < (uint32_t)(10 * 1024)) {
    printf("%d", dividedSize + getRemainingBy1024String(sizeInK, buf));
    printf("%sM", buf);
  } else {
    sizeInK >>= 10;
    dividedSize = sizeInK >> 10;
    printf("%d", dividedSize + getRemainingBy1024String(sizeInK, buf));
    printf("%sG", buf);
  }
}

byte getRemainingBy1024String(uint32_t value, char *destination) {
  byte remaining2;
  char remainingDigit;

  int remaining = value & 0x3FF;
  if (remaining >= 950) {
    *destination = '\0';
    return 1;
  }
  remaining2 = remaining % 100;
  remainingDigit = (remaining / 100) + '0';
  if (remaining2 >= 50) {
    remainingDigit++;
  }

  if (remainingDigit == '0') {
    *destination = '\0';
  } else {
    destination[0] = '.';
    destination[1] = remainingDigit;
    destination[2] = '\0';
  }

  return 0;
}
