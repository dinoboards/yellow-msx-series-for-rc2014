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
partitionInfo    partitions[MAX_PARTITIONS_TO_HANDLE];

// SELECTED ITEMS
msxdosDriverInfo *pSelectedDriverInfo;
deviceInfo *      pSelectedDeviceInfo;
msxddosLunInfo *  pSelectedLunInfo;

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

      printf(" %d: Total Sectors %ld, Capacity: ", count, pLunInfo->sectorCount);
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

//   printf("FDISK Utility\r\n");

//   uint8_t installedDriverCount = retrieveAllDriverInfos();

//   printf("Enter driver number: ");
//   fgets(input, sizeof(input), stdin);
//   uint8_t selectedDriverNumber = atoi(input);
//   pSelectedDriverInfo = &drivers[selectedDriverNumber - 1];

//   retrieveAllDeviceInfo();

//   printf("Enter device number: ");
//   fgets(input, sizeof(input), stdin);
//   uint8_t selectedDeviceNumber = atoi(input);
//   pSelectedDeviceInfo = &devices[selectedDeviceNumber - 1];

//   retrieveLunInfors();
//   printf("Enter logical unit number: ");
//   fgets(input, sizeof(input), stdin);
//   uint8_t selectedLun = atoi(input);
//   pSelectedLunInfo = &luns[selectedLun - 1];

//   preparePartitionAnalysis();
//   uint8_t error = getDiskPartitionsInfo();

//   printf("Error '%d: %s' when searching for patitions.\r\nProceed to manage (y/n)\r\n", error, getDosErrorMessage(error));
//   fgets(input, sizeof(input), stdin);
//   if (input[0] != 'y')
//     exit(0);

//   printf("Proceeding\r\n");

//   partitionState.partitionsExistInDisk = partitionState.partitionsCount > 0;

//   if (!partitionState.partitionsExistInDisk) {
//     printf("Unpartitionned space available: ");
//     printSize(partitionState.unpartitionnedSpaceInSectors / 2);
//     printf("\r\n");
//   }

//   if (partitionState.partitionsCount > 0)
//     printf("S. Show partitions (%d %s)\r\nD. Delete all partitions\r\n", partitionState.partitionsCount, partitionState.partitionsExistInDisk ? "found" : "defined");
//   else if (partitionState.canCreate) {
//     printf("(No partitions found or defined)\r\n");
//   }

//   partitionState.canAddNow = !partitionState.partitionsExistInDisk && partitionState.canCreate && partitionState.unpartitionnedSpaceInSectors >= (MIN_REMAINING_SIZE_FOR_NEW_PARTITIONS_IN_K * 2) + (EXTRA_PARTITION_SECTORS) &&
//                              partitionState.partitionsCount < MAX_PARTITIONS_TO_HANDLE;

//   if (partitionState.canAddNow) {
//     printf("A. Add one ");
//     printSize(partitionState.autoSizeInK);
//     printf(" partition\r\n");
//     printf("P. Add partition...\r\n");
//   }

//   if (!partitionState.partitionsExistInDisk && partitionState.partitionsCount > 0) {
//     printf("U. Undo add ");
//     printSize(partitions[partitionState.partitionsCount - 1].sizeInK);
//     printf(" partition\r\n");
//   }
//   printf("\r\n");

//   if (partitionState.canDoDirectFormat)
//     printf("F. Format device without partitions\r\n\r\n");

//   if (!partitionState.partitionsExistInDisk && partitionState.partitionsCount > 0)
//     printf("W. Write partitions to disk\r\n\r\n");

//   printf("T. Test device access\r\n");
// }

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

void preparePartitionAnalysis() {
  partitionState.pLunInfo = pSelectedLunInfo;
  partitionState.canCreate = (pSelectedLunInfo->sectorCount >= (MIN_DEVICE_SIZE_FOR_PARTITIONS_IN_K * 2));
  partitionState.canDoDirectFormat = (pSelectedLunInfo->sectorCount <= MAX_DEVICE_SIZE_FOR_DIRECT_FORMAT_IN_K * 2);
  printf("\r\n???? %ld, %ld, %d\r\n", pSelectedLunInfo->sectorCount, MAX_DEVICE_SIZE_FOR_DIRECT_FORMAT_IN_K * 2, partitionState.canDoDirectFormat);
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

uint8_t getRemainingBy1024String(uint32_t value, char *destination) {
  uint8_t remaining2;
  char    remainingDigit;

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

uint8_t getDiskPartitionsInfo() {
  uint8_t        primaryIndex = 1;
  uint8_t        extendedIndex = 0;
  uint8_t        error;
  partitionInfo *currentPartition = &partitions[0];

  partitionState.partitionsCount = 0;

  GPartInfo result;

  do {
    error = msxdosGpartInfo(pSelectedDriverInfo->slot, pSelectedDeviceInfo->number, pSelectedLunInfo->number, primaryIndex, extendedIndex, false, &result);

    if (error == 0) {
      if (result.typeCode == PARTYPE_EXTENDED) {
        extendedIndex = 1;
      } else {
        currentPartition->primaryIndex = primaryIndex;
        currentPartition->extendedIndex = extendedIndex;
        currentPartition->partitionType = result.typeCode;
        currentPartition->status = result.status;
        currentPartition->sizeInK = result.sectorCount / 2;
        partitionState.partitionsCount++;
        currentPartition++;
        extendedIndex++;
      }
    } else if (error == _IPART) {
      primaryIndex++;
      extendedIndex = 0;
    } else {
      return error;
    }
  } while (primaryIndex <= 4 && partitionState.partitionsCount < MAX_PARTITIONS_TO_HANDLE);

  return 0;
}

char buffer[MAX_ERROR_EXPLAIN_LENGTH];

const char *getDosErrorMessage(uint8_t code) {
  msxdosExplain(code, buffer);
  return buffer;
}

// ATTEMPT AT ORIGINAL CODE

screenConfiguration currentScreenConfig;
screenConfiguration originalScreenConfig;
uint8_t             screenLinesCount;

void saveOriginalScreenConfiguration() {
  originalScreenConfig.screenMode = *(uint8_t *)SCRMOD;
  originalScreenConfig.screenWidth = *(uint8_t *)LINLEN;
  originalScreenConfig.functionKeysVisible = (*(uint8_t *)CNSDFG != 0);
}

void composeWorkScreenConfiguration() {
  currentScreenConfig.screenMode = 0;
  currentScreenConfig.screenWidth = 80;
  currentScreenConfig.functionKeysVisible = false;
  screenLinesCount = *(uint8_t *)CRTCNT;
}

void setScreenConfiguration(screenConfiguration *screenConfig) {
  *((uint8_t *)LINL40) = 80; // screenConfig->screenWidth;
  msxbiosInitxt();
}

#define clearScreen() printf("\x0C")
#define homeCursor()  printf("\x0D\x1BK")
#define cursorDown()  printf("\x1F")

void locateX(uint8_t x) { msxbiosPosit(x + 1, *(uint8_t *)CSRY); }

void locate(uint8_t x, uint8_t y) { msxbiosPosit(x + 1, y + 1); }
void printCentered(char *string) {
  uint8_t pos = (currentScreenConfig.screenWidth - strlen(string)) / 2;
  homeCursor();
  locateX(pos);
  printf(string);
}

void printRuler() {
  uint8_t i;

  homeCursor();
  for (i = 0; i < currentScreenConfig.screenWidth; i++)
    printf("-");
}

void initializeWorkingScreen(char *header) {
  clearScreen();
  printCentered(header);
  cursorDown();
  printRuler();
  locate(0, screenLinesCount - 2);
  printRuler();
}

void main() {
  printf("1: ???? %d", *((uint8_t *)LINL32));

  saveOriginalScreenConfiguration();

  composeWorkScreenConfiguration();
  setScreenConfiguration(&currentScreenConfig);
  initializeWorkingScreen("Nextor disk partitioning tool");

  // GoDriverSelectionScreen();

  for (uint32_t i = 0; i < 100000; i++)
    ;

  // setScreenConfiguration(&originalScreenConfig);
}
