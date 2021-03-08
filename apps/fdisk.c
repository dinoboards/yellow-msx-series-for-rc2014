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

void terminateRightPaddedStringWithZero(char *string, uint8_t length) {
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

// uint8_t getDiskPartitionsInfo() {
//   uint8_t        primaryIndex = 1;
//   uint8_t        extendedIndex = 0;
//   uint8_t        error;
//   partitionInfo *currentPartition = &partitions[0];

//   partitionState.partitionsCount = 0;

//   GPartInfo result;

//   do {
//     error = msxdosGpartInfo(pSelectedDriverInfo->slot, pSelectedDeviceInfo->number, pSelectedLunInfo->number, primaryIndex, extendedIndex, false, &result);

//     if (error == 0) {
//       if (result.typeCode == PARTYPE_EXTENDED) {
//         extendedIndex = 1;
//       } else {
//         currentPartition->primaryIndex = primaryIndex;
//         currentPartition->extendedIndex = extendedIndex;
//         currentPartition->partitionType = result.typeCode;
//         currentPartition->status = result.status;
//         currentPartition->sizeInK = result.sectorCount / 2;
//         partitionState.partitionsCount++;
//         currentPartition++;
//         extendedIndex++;
//       }
//     } else if (error == _IPART) {
//       primaryIndex++;
//       extendedIndex = 0;
//     } else {
//       return error;
//     }
//   } while (primaryIndex <= 4 && partitionState.partitionsCount < MAX_PARTITIONS_TO_HANDLE);

//   return 0;
// }

char buffer[MAX_ERROR_EXPLAIN_LENGTH];

const char *getDosErrorMessage(uint8_t code) {
  msxdosExplain(code, buffer);
  return buffer;
}

// ATTEMPT AT ORIGINAL CODE

screenConfiguration currentScreenConfig;
screenConfiguration originalScreenConfig;
uint8_t             screenLinesCount;
uint8_t             installedDriversCount;

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

void setScreenConfiguration() {
  *((uint8_t *)LINL40) = 80; // screenConfig->screenWidth;
  msxbiosInitxt();
}

#define clearScreen()                    printf("\x0C")
#define homeCursor()                     printf("\x0D\x1BK")
#define cursorDown()                     printf("\x1F")
#define deleteToEndOfLine()              printf("\x1BK")
#define deleteToEndOfLineAndCursorDown() printf("\x1BK\x1F");
#define newLine()                        printf("\x0A\x0D");

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

void PrintStateMessage(char *string) {
  locate(0, screenLinesCount - 1);
  deleteToEndOfLine();
  printf(string);
}

char getKey() { return msxdosDirio(0xFF); }

uint8_t waitKey() {
  uint8_t key;

  while ((key = getKey()) == 0)
    ;
  return key;
}

void clearInformationArea() {
  uint8_t i;

  locate(0, 2);
  for (i = 0; i < screenLinesCount - 4; i++) {
    deleteToEndOfLineAndCursorDown();
  }
}

void getDriversInformation() {
  uint8_t           error = 0;
  uint8_t           driverIndex = 1;
  msxdosDriverInfo *currentDriver = &drivers[0];

  installedDriversCount = 0;

  while (error == 0 && driverIndex <= MAX_INSTALLED_DRIVERS) {
    error = msxdosGdrvr(driverIndex, currentDriver);

    if (error == 0 && (currentDriver->flags & (DRIVER_IS_DOS250 | DRIVER_IS_DEVICE_BASED) == (DRIVER_IS_DOS250 | DRIVER_IS_DEVICE_BASED))) {
      installedDriversCount++;
      terminateRightPaddedStringWithZero(currentDriver->driverName, DRIVER_NAME_LENGTH);
      currentDriver++;
    }
    driverIndex++;
  }
}

void printStateMessage(char *string) {
  locate(0, screenLinesCount - 1);
  deleteToEndOfLine();
  printf(string);
}

void showDriverSelectionScreen() {
  uint8_t           i;
  char              slot[4];
  char              rev[3];
  msxdosDriverInfo *currentDriver;
  uint8_t           revByte;
  char *            driverName;

  clearInformationArea();

  if (installedDriversCount == 0) {
    getDriversInformation();
  }

  if (installedDriversCount == 0) {
    locate(0, 7);
    printCentered("There are no device-based drivers");
    cursorDown();
    printCentered("available in the system");
    printStateMessage("Press any key to exit...");
    waitKey();
    return;
  }

  currentDriver = &drivers[0];
  locate(0, 3);
  for (i = 0; i < installedDriversCount; i++) {
    composeSlotString(currentDriver->slot, slot);

    revByte = currentDriver->versionRev;
    if (revByte == 0) {
      rev[0] = '\0';
    } else {
      rev[0] = '.';
      rev[1] = revByte + '0';
      rev[2] = '\0';
    }

    driverName = currentDriver->driverName;

    printf("\x1BK%d. %s%sv%d.%d%s on slot %s", i + 1, driverName, " ", currentDriver->versionMain, currentDriver->versionSec, rev, slot);

    newLine();
    newLine();

    currentDriver++;
  }

  newLine();
  printf("ESC. Exit");

  printStateMessage("Select the device driver");
}

void goDriverSelectionScreen() {
  // uint8_t key;

  // while (true) {
    showDriverSelectionScreen();
    if (installedDriversCount == 0) {
      return;
    }

    //     while(true) {
    //         key = WaitKey();
    //         if(key == ESC) {
    //             return;
    //         } else {
    //             key -= '0';
    //             if(key >= 1 && key <= installedDriversCount) {
    //                 GoDeviceSelectionScreen(key);
    //                 break;
    //             }
    //         }
    //     }
    // }
  // }
}

void main() {
  saveOriginalScreenConfiguration();

  composeWorkScreenConfiguration();
  setScreenConfiguration();
  initializeWorkingScreen("Nextor disk partitioning tool");

  goDriverSelectionScreen();

  for (uint32_t i = 0; i < 600000; i++)
    ;

  // setScreenConfiguration(&originalScreenConfig);
}
