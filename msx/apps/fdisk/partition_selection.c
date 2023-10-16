#include "partition_selection.h"
#include "device_selection.h"
#include "driver_selection.h"
#include "key_inputs.h"
#include "lun_selection.h"
#include "screen_control.h"
#include "temp.h"
#include <partition.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

partitionInfo partitions[MAX_PARTITIONS_TO_HANDLE];
uint32_t      autoPartitionSizeInK;

void addAutoPartition(void) {
  partitionInfo *partition = &partitions[partitionsCount];

  partition->status        = partitionsCount == 0 ? 0x80 : 0;
  partition->sizeInK       = autoPartitionSizeInK;
  partition->partitionType = partition->sizeInK > MAX_FAT12_PARTITION_SIZE_IN_K ? PARTYPE_FAT16_LBA : PARTYPE_FAT12;
  if (partitionsCount == 0) {
    partition->primaryIndex  = 1;
    partition->extendedIndex = 0;
  } else {
    partition->primaryIndex  = 2;
    partition->extendedIndex = partitionsCount;
  }

  unpartitionnedSpaceInSectors -= (autoPartitionSizeInK * 2);
  unpartitionnedSpaceInSectors -= EXTRA_PARTITION_SECTORS;
  partitionsCount++;
  recalculateAutoPartitionSize(false);
}

void addPartition(void) {
  uint16_t maxPartitionSizeInM;
  uint16_t maxPartitionSizeInK;
  uint8_t  lineLength;
  char    *pointer;
  char     ch;
  bool     validNumberEntered = false;
  uint32_t enteredSizeInK     = 0;
  bool     lessThan1MAvailable;
  bool     sizeInKSpecified;
  uint32_t unpartitionnedSpaceExceptAlignmentInK = (unpartitionnedSpaceInSectors - EXTRA_PARTITION_SECTORS) / 2;

  maxPartitionSizeInM = (uint16_t)((unpartitionnedSpaceInSectors / 2) >> 10);
  maxPartitionSizeInK =
      unpartitionnedSpaceExceptAlignmentInK > (uint32_t)32767 ? (uint16_t)32767 : unpartitionnedSpaceExceptAlignmentInK;

  lessThan1MAvailable = (maxPartitionSizeInM == 0);

  if (maxPartitionSizeInM > (uint32_t)MAX_FAT16_PARTITION_SIZE_IN_M) {
    maxPartitionSizeInM = MAX_FAT16_PARTITION_SIZE_IN_M;
  }

  printStateMessage("Enter size or press ENTER to cancel");

  while (!validNumberEntered) {
    sizeInKSpecified = true;
    clearInformationArea();
    printTargetInfo();
    newLine();
    printf("Add new partition\r\n\r\n");

    if (lessThan1MAvailable) {
      printf("Enter");
    } else {
      printf("Enter partition size in MB (1-%d)\r\nor", maxPartitionSizeInM);
    }
    printf(" partition size in KB followed by \"K\" (%d-%d): ", MIN_PARTITION_SIZE_IN_K, maxPartitionSizeInK);

    fgets(buffer, 6, stdin);
    lineLength = strlen(buffer) - 1; // remove CR
    if (lineLength == 0) {
      return;
    }

    pointer             = buffer;
    pointer[lineLength] = '\0';
    enteredSizeInK      = 0;

    while (true) {
      ch = (*pointer++) | 32;
      if (ch == 'k') {
        validNumberEntered = true;
        break;
      } else if (ch == '\0' || ch == 13 || ch == 'm') {
        validNumberEntered = true;
        enteredSizeInK <<= 10;
        sizeInKSpecified = false;
        break;
      } else if (ch < '0' || ch > '9') {
        break;
      }

      enteredSizeInK = (enteredSizeInK * 10) + (ch - '0');
      lineLength--;
      if (lineLength == 0) {
        validNumberEntered = true;
        enteredSizeInK *= 1024;
        sizeInKSpecified = false;
        break;
      }
    }

    if (validNumberEntered &&
            (sizeInKSpecified && (enteredSizeInK > maxPartitionSizeInK) || (enteredSizeInK < MIN_PARTITION_SIZE_IN_K)) ||
        (!sizeInKSpecified && (enteredSizeInK > ((uint32_t)maxPartitionSizeInM * 1024)))) {
      validNumberEntered = false;
    }
  }

  autoPartitionSizeInK =
      enteredSizeInK > unpartitionnedSpaceExceptAlignmentInK ? unpartitionnedSpaceExceptAlignmentInK : enteredSizeInK;
  addAutoPartition();
  unpartitionnedSpaceExceptAlignmentInK = (unpartitionnedSpaceInSectors - EXTRA_PARTITION_SECTORS) / 2;
  autoPartitionSizeInK =
      enteredSizeInK > unpartitionnedSpaceExceptAlignmentInK ? unpartitionnedSpaceExceptAlignmentInK : enteredSizeInK;
  recalculateAutoPartitionSize(false);
}

void deleteAllPartitions(void) {
  sprintf(buffer, "Discard all %s partitions? (y/n) ", partitionsExistInDisk ? "existing" : "defined");
  printStateMessage(buffer);
  if (!getYesOrNo()) {
    return;
  }

  partitionsCount              = 0;
  partitionsExistInDisk        = false;
  unpartitionnedSpaceInSectors = selectedLun->sectorCount;
  recalculateAutoPartitionSize(true);
}

void togglePartitionActive(uint8_t partitionIndex) {
  uint8_t        status, primaryIndex, extendedIndex;
  partitionInfo *partition;
  uint32_t       partitionTableEntrySector;
  uint8_t        error;
  GPartInfo      result;

  partition = &partitions[partitionIndex];

  if (!partitionsExistInDisk) {
    partition->status ^= 0x80;
    return;
  }

  status        = partition->status;
  primaryIndex  = partition->primaryIndex;
  extendedIndex = partition->extendedIndex;

  sprintf(buffer, "%set active bit of partition %d? (y/n) ", status & 0x80 ? "Res" : "S", partitionIndex + 1);
  printStateMessage(buffer);
  if (!getYesOrNo()) {
    return;
  }

  error = msxdosGpart(selectedDriver->slot, selectedDeviceIndex, selectedLunIndex + 1, partition->primaryIndex,
                      partition->extendedIndex, true, &result);
  if (error != 0)
    return;

  partitionTableEntrySector = result.partitionSector;

  preparePartitioningProcess(selectedDriver->slot, selectedDeviceIndex, selectedLunIndex + 1, partitionsCount, partitions,
                             luns[selectedLunIndex].sectorsPerTrack);

  error = toggleStatusBit(extendedIndex == 0 ? primaryIndex - 1 : 0, partitionTableEntrySector);
  if (error == 0) {
    partition->status ^= 0x80;
  } else {
    sprintf(buffer, "Error when accessing device: %d", error);
    clearInformationArea();
    locate(0, 7);
    printCentered(buffer);
    printStateMessage("Press any key...");
    waitKey();
  }
}

void printOnePartitionInfo(partitionInfo *info) {
  printf("%c %d: ", info->status & 0x80 ? '*' : ' ', info->extendedIndex == 0 ? info->primaryIndex : info->extendedIndex + 1);

  if (info->partitionType == PARTYPE_FAT12) {
    printf("FAT12");
  } else if (info->partitionType == PARTYPE_FAT16 || info->partitionType == PARTYPE_FAT16_SMALL ||
             info->partitionType == PARTYPE_FAT16_LBA) {
    printf("FAT16");
  } else if (info->partitionType == 0xB || info->partitionType == 0xC) {
    printf("FAT32");
  } else if (info->partitionType == 7) {
    printf("NTFS");
  } else {
    printf("Type #%x", info->partitionType);
  }
  printf(", ");
  printSize(info->sizeInK);
  newLine();
}

void showPartitions(void) {
  int            i;
  int            firstShownPartitionIndex = 1;
  int            lastPartitionIndexToShow;
  bool           isLastPage;
  bool           isFirstPage;
  bool           allPartitionsArePrimary;
  uint8_t        key;
  partitionInfo *currentPartition;

  if (partitionsExistInDisk) {
    allPartitionsArePrimary = true;
    for (i = 0; i < partitionsCount; i++) {
      currentPartition = &partitions[i];
      if (currentPartition->extendedIndex != 0) {
        allPartitionsArePrimary = false;
        break;
      }
    }
  } else {
    allPartitionsArePrimary = false;
  }

  while (true) {
    isFirstPage              = (firstShownPartitionIndex == 1);
    isLastPage               = (firstShownPartitionIndex + PARTITIONS_PER_PAGE) > partitionsCount;
    lastPartitionIndexToShow = isLastPage ? partitionsCount : firstShownPartitionIndex + PARTITIONS_PER_PAGE - 1;

    locate(0, screenLinesCount - 1);
    deleteToEndOfLine();
    if (isFirstPage) {
      sprintf(buffer, partitionsCount == 1 ? "1" : partitionsCount > 9 ? "1-9" : "1-%d", partitionsCount);
      if (isLastPage) {
        sprintf(buffer + 4, "ESC = return, %s = toggle active (*)", buffer);
      } else {
        sprintf(buffer + 4, "ESC=back, %s=toggle active (*)", buffer);
      }
      printCentered(buffer + 4);
    } else {
      printCentered("Press ESC to return");
    }

    if (!(isFirstPage && isLastPage)) {
      locate(0, screenLinesCount - 1);
      printf(isFirstPage ? "   " : "<--");

      locate(currentScreenConfig.screenWidth - 4, screenLinesCount - 1);
      printf(isLastPage ? "   " : "-->");
    }

    clearInformationArea();
    locate(0, 3);
    if (partitionsCount == 1) {
      printCentered(partitionsExistInDisk ? "One partition found on device" : "One new partition defined");
    } else {
      if (allPartitionsArePrimary) {
        sprintf(buffer, partitionsExistInDisk ? "%d primary partitions found on device" : "%d new primary partitions defined",
                partitionsCount);
      } else {
        sprintf(buffer, partitionsExistInDisk ? "%d partitions found on device" : "%d new partitions defined", partitionsCount);
      }
      printCentered(buffer);
    }
    newLine();
    if (partitionsCount > PARTITIONS_PER_PAGE) {
      sprintf(buffer, "Displaying partitions %d - %d", firstShownPartitionIndex, lastPartitionIndexToShow);
      printCentered(buffer);
      newLine();
    }
    newLine();

    currentPartition = &partitions[firstShownPartitionIndex - 1];

    for (i = firstShownPartitionIndex; i <= lastPartitionIndexToShow; i++) {
      printOnePartitionInfo(currentPartition);
      currentPartition++;
    }

    while (true) {
      key = waitKey();
      if (key == ESC) {
        return;
      } else if (key == CURSOR_LEFT && !isFirstPage) {
        firstShownPartitionIndex -= PARTITIONS_PER_PAGE;
        break;
      } else if (key == CURSOR_RIGHT && !isLastPage) {
        firstShownPartitionIndex += PARTITIONS_PER_PAGE;
        break;
      } else if (isFirstPage && key >= KEY_1 && key < KEY_1 + partitionsCount && key < KEY_1 + 9) {
        togglePartitionActive(key - KEY_1);
        break;
      }
    }
  }
}

uint8_t getDiskPartitionsInfo(void) {
  uint8_t        primaryIndex  = 1;
  uint8_t        extendedIndex = 0;
  uint8_t        error;
  partitionInfo *currentPartition = &partitions[0];
  GPartInfo      result;

  partitionsCount = 0;

  do {
    result.sectorCount = 0;
    error =
        msxdosGpart(selectedDriver->slot, selectedDeviceIndex, selectedLunIndex + 1, primaryIndex, extendedIndex, false, &result);

    if (error == 0) {
      if (result.typeCode == PARTYPE_EXTENDED)
        extendedIndex = 1;
      else {
        currentPartition->primaryIndex  = primaryIndex;
        currentPartition->extendedIndex = extendedIndex;
        currentPartition->partitionType = result.typeCode;
        currentPartition->status        = result.status;
        currentPartition->sizeInK       = result.sectorCount / 2;
        partitionsCount++;
        currentPartition++;
        extendedIndex++;
      }
    } else if (error == _IPART) {
      primaryIndex++;
      extendedIndex = 0;
    } else
      return error;

  } while (primaryIndex <= 4 && partitionsCount < MAX_PARTITIONS_TO_HANDLE);

  return 0;
}

void goPartitioningMainMenuScreen(void) {
  char    key;
  uint8_t error;
  bool    canAddPartitionsNow;
  bool    mustRetrievePartitionInfo = true;

  while (true) {
    if (mustRetrievePartitionInfo) {
      clearInformationArea();
      printTargetInfo();

      if (canCreatePartitions) {
        locate(0, MESSAGE_ROW);
        printCentered("Searching partitions...");
        printStateMessage("Please wait...");
        error = getDiskPartitionsInfo();
        if (error != 0) {
          printDosErrorMessage(error, "Error when searching partitions:");
          printStateMessage("Manage device anyway? (y/n) ");
          if (!getYesOrNo())
            return;
        }
        partitionsExistInDisk = (partitionsCount > 0);
      }
      mustRetrievePartitionInfo = false;
    }

    clearInformationArea();
    printTargetInfo();
    if (!partitionsExistInDisk) {
      printf("Unpartitionned space available: ");
      printSize(unpartitionnedSpaceInSectors / 2);
      newLine();
    }
    newLine();

    printf("Changes are not committed until W is pressed.\r\n\r\n");

    if (partitionsCount > 0) {
      printf("S. Show partitions (%d %s)\r\n"
             "D. Delete all partitions\r\n",
             partitionsCount, partitionsExistInDisk ? "found" : "defined");
    } else if (canCreatePartitions) {
      printf("(No partitions found or defined)\r\n");
    }
    canAddPartitionsNow =
        !partitionsExistInDisk && canCreatePartitions &&
        unpartitionnedSpaceInSectors >= (MIN_REMAINING_SIZE_FOR_NEW_PARTITIONS_IN_K * 2) + (EXTRA_PARTITION_SECTORS) &&
        partitionsCount < MAX_PARTITIONS_TO_HANDLE;
    if (canAddPartitionsNow) {
      printf("A. Add one ");
      printSize(autoPartitionSizeInK);
      printf(" partition\r\n");
      printf("P. Add partition...\r\n");
    }
    if (!partitionsExistInDisk && partitionsCount > 0) {
      printf("U. Undo add ");
      printSize(partitions[partitionsCount - 1].sizeInK);
      printf(" partition\r\n");
    }
    newLine();
    if (canDoDirectFormat) {
      printf("F. Format device without partitions\r\n\r\n");
    }
    if (!partitionsExistInDisk && partitionsCount > 0) {
      printf("W. Write partitions to disk\r\n\r\n");
    }
    printf("T. Test device access\r\n");
    printf("C: Test write for last sector\r\n");
    printf("X: Test write all sectors\r\n");

    printStateMessage("Select an option or press ESC to return");

    while ((key = waitKey()) == 0)
      ;
    if (key == ESC) {
      if (partitionsExistInDisk || partitionsCount == 0) {
        return;
      }
      printStateMessage("Discard changes and return? (y/n) ");
      if (getYesOrNo()) {
        return;
      } else {
        continue;
      }
    }
    key |= 32;
    if (key == 's' && partitionsCount > 0) {
      showPartitions();
      continue;
    }

    if (key == 'd' && partitionsCount > 0) {
      deleteAllPartitions();
      continue;
    }

    if (key == 'p' && canAddPartitionsNow > 0) {
      addPartition();
      continue;
    }

    if (key == 'a' && canAddPartitionsNow > 0) {
      addAutoPartition();
      continue;
    }
    // } else if(key == 'u' && !partitionsExistInDisk && partitionsCount > 0) {
    // 	UndoAddPartition();
    if (key == 't') {
      testDeviceAccess();
      continue;
    }

    if (key == 'c') {
      testDeviceWriteAccess();
      continue;
    }

    if (key == 'x') {
      testDeviceFullWriteAccess();
      continue;
    }

    // } else if(key == 'f' && canDoDirectFormat) {
    // 	if(FormatWithoutPartitions()) {
    // 		mustRetrievePartitionInfo = true;
    // 	}
    if (key == 'w' && !partitionsExistInDisk && partitionsCount > 0)
      if (writePartitionTable())
        mustRetrievePartitionInfo = true;
  }
}
