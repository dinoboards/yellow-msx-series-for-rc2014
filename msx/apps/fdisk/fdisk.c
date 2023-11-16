#include "fdisk.h"
#include "device_selection.h"
#include "driver_selection.h"
#include "fdisk2.h"
#include "key_inputs.h"
#include "lun_selection.h"
#include "msxdos.h"
#include "partition.h"
#include "partition_selection.h"
#include "screen_control.h"
#include "string_helper.h"
#include "temp.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <system_vars.h>

extern uint8_t workingMsxDosBuff[];
uint8_t       *pWorkingBuffer;

// CAPUTED ENUMERATED DRIVER/DEVICE/LUN INFO

uint8_t  partitionsCount;
bool     partitionsExistInDisk;
bool     canCreatePartitions;
bool     canDoDirectFormat;
uint32_t unpartitionnedSpaceInSectors;
char     buffer[1024];

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
  remaining2     = remaining % 100;
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

void printRuler(void) {
  uint8_t i;

  // "Hack" for korean MSX computers that do weird things
  // when printing a character at the last column of the screen
  const byte width = (*((byte *)H_CHPH) != 0xC9) ? currentScreenConfig.screenWidth - 1 : currentScreenConfig.screenWidth;

  homeCursor();
  for (i = 0; i < width; i++) {
    printf("-");
  }
}

void initializeWorkingScreen(char *header) {
  clearScreen();
  printCentered(header);
  cursorDown();
  printRuler();
  locate(0, screenLinesCount - 2);
  printRuler();
}

void clearInformationArea(void) {
  uint8_t i;

  locate(0, 2);
  for (i = 0; i < screenLinesCount - 4; i++) {
    deleteToEndOfLineAndCursorDown();
  }
}

void printStateMessage(char *string) {
  locate(0, screenLinesCount - 1);
  deleteToEndOfLine();
  printf(string);
}

void recalculateAutoPartitionSize(bool setToAllSpaceAvailable) {
  uint32_t maxAbsolutePartitionSizeInK = (unpartitionnedSpaceInSectors - EXTRA_PARTITION_SECTORS) / 2;

  if (setToAllSpaceAvailable) {
    autoPartitionSizeInK = maxAbsolutePartitionSizeInK;
  }

  if (autoPartitionSizeInK > MAX_FAT16_PARTITION_SIZE_IN_K) {
    autoPartitionSizeInK = MAX_FAT16_PARTITION_SIZE_IN_K;
  } else if (!setToAllSpaceAvailable && autoPartitionSizeInK > maxAbsolutePartitionSizeInK) {
    autoPartitionSizeInK = maxAbsolutePartitionSizeInK;
  }

  if (autoPartitionSizeInK < MIN_PARTITION_SIZE_IN_K) {
    autoPartitionSizeInK = MIN_PARTITION_SIZE_IN_K;
  } else if (autoPartitionSizeInK > maxAbsolutePartitionSizeInK) {
    autoPartitionSizeInK = maxAbsolutePartitionSizeInK;
  }
}

void printTargetInfo(void) {
  locate(0, 3);
  printf(selectedDriverName);
  printf(currentDevice->deviceName);
  printDeviceInfoWithIndex();
  newLine();
  printf("Logical unit %d, size: ", selectedLunIndex + 1);
  printSize(selectedLun->sectorCount / 2);
  newLine();
}

void printDosErrorMessage(uint8_t code, char *header) {
  locate(0, MESSAGE_ROW);
  printCentered(header);
  newLine();

  msxdosExplainErrorCode(code, buffer);
  if (strlen(buffer) > currentScreenConfig.screenWidth) {
    printf(buffer);
  } else {
    printCentered(buffer);
  }

  printStateMessage("Press any key to return...");
}

void initializeScreenForTestDeviceAccess(const char *message) {
  clearInformationArea();
  printTargetInfo();
  locate(0, MESSAGE_ROW);
  printf(message);
  printStateMessage("Press any key to stop...");
}

void testDeviceAccess(void) {
  uint32_t    sectorNumber = 0;
  const char *message      = "Now reading device sector ";
  uint8_t     messageLen   = strlen(message);
  uint16_t    error;
  const char *errorMessageHeader = "Error when reading sector ";

  initializeScreenForTestDeviceAccess(message);

  while (getKey() == 0) {
    sprintf(buffer, "%u", sectorNumber);
    locate(messageLen, MESSAGE_ROW);
    printf(buffer);
    printf(" ...\x1BK");

    error = msxdosDevRead(selectedDriver->slot, selectedDeviceIndex, selectedLunIndex + 1, sectorNumber, 2, buffer);

    if (error != 0) {
      strcpy(buffer, errorMessageHeader);
      sprintf(buffer + strlen(errorMessageHeader), "%u", sectorNumber);
      strcpy(buffer + strlen(buffer), ":");
      printDosErrorMessage(error, buffer);
      printStateMessage("Continue reading sectors? (y/n) ");
      if (!getYesOrNo()) {
        return;
      }
      initializeScreenForTestDeviceAccess(message);
    }

    sectorNumber++;
    if (sectorNumber >= selectedLun->sectorCount) {
      sectorNumber = 0;
    }
  }
}

void initializeScreenForTestDeviceWriteAccess(const char *message) {
  clearInformationArea();
  printTargetInfo();
  locate(0, MESSAGE_ROW);
  printf(message);
  printStateMessage("Press any key to return...");
}

bool readSector(uint32_t targetSector) {
  uint16_t error = msxdosDevRead(selectedDriver->slot, selectedDeviceIndex, selectedLunIndex + 1, targetSector, 1, buffer);
  if (error != 0) {
    printDosErrorMessage(error, "Driver Error:");
    return FALSE;
  }

  return TRUE;
}

bool writeSector(uint32_t targetSector) {
  uint16_t error = msxdosDevWrite(selectedDriver->slot, selectedDeviceIndex, selectedLunIndex + 1, targetSector, 1, buffer);
  if (error != 0) {
    printDosErrorMessage(error, "Driver Error:");
    return FALSE;
  }

  return TRUE;
}

#define FOR_BUFFER(f, s)                                                                                                           \
  for (i = 0; i < s; i++)                                                                                                          \
  f
#define CHECK_BUFFER(c, f, s)                                                                                                      \
  FOR_BUFFER(                                                                                                                      \
      if (c) {                                                                                                                     \
        f;                                                                                                                         \
        break;                                                                                                                     \
      },                                                                                                                           \
      s)

void testDeviceWriteAccess(void) {
  uint16_t error;
  uint16_t i;

  initializeScreenForTestDeviceWriteAccess("Checking write access on last sector\r\n");
  locate(0, MESSAGE_ROW + 1);
  printf("WARNING! Potential data corruption.\r\n");
  printStateMessage("Proceed? (y/n) ");
  if (!getYesOrNo())
    return;

  locate(0, MESSAGE_ROW + 2);

  const uint32_t targetSector = selectedLun->sectorCount - 1;

  printf("Target sector %ld\r\n", targetSector);

  printf("Reading\r\n");
  if (!readSector(targetSector))
    goto abortTest;

  FOR_BUFFER(buffer[i] = 0, 512);

  printf("Writing zeros\r\n");
  if (!writeSector(targetSector))
    goto abortTest;

  printf("Verifiying\r\n");
  if (!readSector(targetSector))
    goto abortTest;

  error = FALSE;
  CHECK_BUFFER(buffer[i] != 0, error = TRUE, 512);

  if (error) {
    printf("Comparision failure at byte %d\r\n", i);
    goto abortTest;
  }

  FOR_BUFFER(buffer[i] = i, 512);

  printf("Writing sequence\r\n");
  if (!writeSector(targetSector))
    goto abortTest;

  printf("Verifiying\r\n");
  if (!readSector(targetSector))
    goto abortTest;

  error = FALSE;
  CHECK_BUFFER(buffer[i] != (uint8_t)i, error = TRUE, 512);

  if (error) {
    printf("Comparision failure at byte %d\r\n", i);
    goto abortTest;
  }

  printStateMessage("Success.  Press any key to exit");

abortTest:
  waitKey();
}

// const char fullWriteTestMessage[] = "Full Write Test";

bool readWriteError(const char *errorMessageHeader, const uint16_t error, const uint16_t sectorNumber, const char *message) {
  strcpy(buffer, errorMessageHeader);
  sprintf(buffer + strlen(errorMessageHeader), "%u", sectorNumber);
  strcpy(buffer + strlen(buffer), ":");
  printDosErrorMessage(error, buffer);
  printStateMessage("Continue reading sectors? (y/n) ");
  if (!getYesOrNo()) {
    return false;
  }
  initializeScreenForTestDeviceAccess(message);

  return true;
}

void increment32Bit(uint32_t *p) __z88dk_fastcall { (*p)++; }

void testDeviceFullWriteAccess(void) {
  const char *message      = "Now testing sector ";
  uint32_t    sectorNumber = 0;
  uint8_t     messageLen   = strlen(message);
  uint16_t    error        = 23;
  uint16_t    i;

  initializeScreenForTestDeviceWriteAccess("Checking write access on *ALL* sector\r\n");
  locate(0, MESSAGE_ROW + 1);
  printf("WARNING! All existing data will be lost.\r\n");
  printStateMessage("Proceed? (y/n) ");
  if (!getYesOrNo())
    return;

  initializeScreenForTestDeviceAccess(message);

  while (getKey() == 0) {
    sprintf(buffer, "%u", sectorNumber);
    locate(messageLen, MESSAGE_ROW);
    printf(buffer);
    printf(" ...\x1BK");

    FOR_BUFFER(buffer[i] = 0, 512);

    error = msxdosDevWrite(selectedDriver->slot, selectedDeviceIndex, selectedLunIndex + 1, sectorNumber, 1, buffer);
    if (error) {
      if (!readWriteError("Error when writing zeros to sector ", error, sectorNumber, message))
        return;
      goto nextSector;
    }

    FOR_BUFFER(buffer[i] = 255, 512);

    error = msxdosDevRead(selectedDriver->slot, selectedDeviceIndex, selectedLunIndex + 1, sectorNumber, 1, buffer);
    if (error) {
      if (!readWriteError("Error when re-reading sector ", error, sectorNumber, message))
        return;
      goto nextSector;
    }

    error = FALSE;
    CHECK_BUFFER(buffer[i] != 0, error = TRUE, 512);
    if (error) {
      if (!readWriteError("Verification failed for sector ", error, sectorNumber, message))
        return;
      goto nextSector;
    }

    FOR_BUFFER(buffer[i] = i, 1024);
    error = msxdosDevWrite(selectedDriver->slot, selectedDeviceIndex, selectedLunIndex + 1, sectorNumber, 2, buffer);
    if (error) {
      if (!readWriteError("Error when writing sequence to sector ", error, sectorNumber, message))
        return;
      goto nextSector;
    }

    FOR_BUFFER(buffer[i] = 0, 1024);
    error = msxdosDevRead(selectedDriver->slot, selectedDeviceIndex, selectedLunIndex + 1, sectorNumber, 2, buffer);
    if (error) {
      if (!readWriteError("Error when reading sequence sector ", error, sectorNumber, message))
        return;
      goto nextSector;
    }

    error = FALSE;
    CHECK_BUFFER(buffer[i] != (uint8_t)i, error = TRUE, 1024);
    if (error) {
      if (!readWriteError("Sequence verification failed for sector ", error, sectorNumber, message))
        return;
      goto nextSector;
    }

  nextSector:
    sectorNumber++;
    if (sectorNumber >= selectedLun->sectorCount) {
      sectorNumber = 0;
    }
  }

  printStateMessage("Success.  Press any key to exit");

abortTest:
  waitKey();
}

bool confirmDataDestroy(char *action) {
  printStateMessage("");
  clearInformationArea();
  printTargetInfo();
  locate(0, MESSAGE_ROW);

  printf("%s\r\n"
         "\r\n"
         "THIS WILL DESTROY ALL DATA ON THE DEVICE!!\r\n"
         "This action can't be cancelled and can't be undone\r\n"
         "\r\n"
         "Are you sure? (y/n) ",
         action);

  return getYesOrNo();
}

void printDone(void) {
  printCentered("Done!");
  printf("\x0A\x0D\x0A\x0A\x0A");
  printCentered("If this device had drives mapped,");
  newLine();
  printCentered("please reset the computer.");
}

bool writePartitionTable(void) {
  uint8_t i;
  uint8_t error = 0;

  sprintf(buffer, "Create %d partitions on device", partitionsCount);

  if (!confirmDataDestroy(buffer))
    return false;

  clearInformationArea();
  printTargetInfo();
  printStateMessage("Please wait...");

  locate(0, MESSAGE_ROW);
  printCentered("Preparing partitioning process...");

  preparePartitioningProcess(selectedDriver->slot, selectedDeviceIndex, selectedLunIndex + 1, partitionsCount, partitions,
                             luns[selectedLunIndex].sectorsPerTrack);

  for (i = 0; i < partitionsCount; i++) {
    locate(0, MESSAGE_ROW);
    sprintf(buffer, "Creating partition %d of %d ...", i + 1, partitionsCount);
    printCentered(buffer);

    error = createPartition(i);
    if (error != 0) {
      sprintf(buffer, "Error when creating partition %d :", i + 1);
      printDosErrorMessage(error, buffer);
      waitKey();
      return false;
    }
  }

  locate(0, MESSAGE_ROW + 2);
  printDone();
  printStateMessage("Press any key to return...");
  waitKey();

  return true;
}

void main(void) {
  installedDriversCount = 0;
  selectedDeviceIndex   = 0;
  selectedLunIndex      = 0;
  availableLunsCount    = 0;

  saveOriginalScreenConfiguration();

  composeWorkScreenConfiguration();
  setScreenConfiguration();
  initializeWorkingScreen("Nextor disk partitioning tool");

  goDriverSelectionScreen();
}
