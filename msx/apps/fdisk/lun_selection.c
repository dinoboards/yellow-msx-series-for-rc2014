#include "lun_selection.h"
#include "device_selection.h"
#include "driver_selection.h"
#include "key_inputs.h"
#include "partition_selection.h"
#include "screen_control.h"
#include "temp.h"
#include <partition.h>
#include <stdio.h>

msxdosLunInfo  luns[MAX_LUNS_PER_DEVICE];
uint8_t        availableLunsCount;
uint8_t        selectedLunIndex;
msxdosLunInfo *selectedLun;

void initializePartitioningVariables(uint8_t lunIndex) {
  selectedLunIndex             = lunIndex - 1;
  selectedLun                  = &luns[selectedLunIndex];
  partitionsCount              = 0;
  partitionsExistInDisk        = true;
  canCreatePartitions          = (selectedLun->sectorCount >= (MIN_DEVICE_SIZE_FOR_PARTITIONS_IN_K * 2));
  canDoDirectFormat            = (selectedLun->sectorCount <= MAX_DEVICE_SIZE_FOR_DIRECT_FORMAT_IN_K * 2);
  unpartitionnedSpaceInSectors = selectedLun->sectorCount;
  recalculateAutoPartitionSize(true);
}

void getLunsInformation(void) {
  uint16_t       error      = 0;
  uint8_t        lunIndex   = 1;
  msxdosLunInfo *currentLun = &luns[0];

  while (lunIndex <= MAX_LUNS_PER_DEVICE) {
    error = msxdosDrvLunInfo(selectedDriver->slot, selectedDeviceIndex, lunIndex, currentLun);

    currentLun->suitableForPartitioning =
        (error == 0) && (currentLun->mediumType == BLOCK_DEVICE) && (currentLun->sectorSize == 512) &&
        (currentLun->sectorCount >= MIN_DEVICE_SIZE_IN_K * 2) && ((currentLun->flags & (READ_ONLY_LUN | FLOPPY_DISK_LUN)) == 0);

    if (currentLun->suitableForPartitioning) {
      availableLunsCount++;
    }

    if (currentLun->sectorsPerTrack == 0 || currentLun->sectorsPerTrack > EXTRA_PARTITION_SECTORS) {
      currentLun->sectorsPerTrack = EXTRA_PARTITION_SECTORS;
    }

    lunIndex++;
    currentLun++;
  }
}

void showLunSelectionScreen(void) {
  uint8_t        i;
  msxdosLunInfo *currentLun;

  clearInformationArea();
  locate(0, 3);
  printf(selectedDriverName);
  printf(currentDevice->deviceName);
  printDeviceInfoWithIndex();
  newLine();
  newLine();
  newLine();

  if (availableLunsCount == 0) {
    getLunsInformation();
  }

  if (availableLunsCount == 0) {
    locate(0, 9);
    printCentered("There are no suitable logical units");
    cursorDown();
    printCentered("available in the device");
    printStateMessage("Press any key to go back...");
    waitKey();
    return;
  }

  currentLun = &luns[0];
  for (i = 0; i < MAX_LUNS_PER_DEVICE; i++) {
    if (currentLun->suitableForPartitioning) {
      printf("\x1BK%d. Size: ", i + 1);
      printSize(currentLun->sectorCount / 2);
      newLine();
    }

    i++;
    currentLun++;
  }

  newLine();
  newLine();
  printf("ESC. Go back to device selection screen");

  printStateMessage("Select the logical unit");
}

void goLunSelectionScreen(uint8_t deviceIndex) {
  uint8_t key;

  currentDevice       = &devices[deviceIndex - 1];
  selectedDeviceIndex = deviceIndex;

  availableLunsCount = 0;

  while (true) {
    showLunSelectionScreen();
    if (availableLunsCount == 0) {
      return;
    }

    while (true) {
      key = waitKey();
      if (key == ESC)
        return;

      key -= '0';
      if (key >= 1 && key <= MAX_LUNS_PER_DEVICE && luns[key - 1].suitableForPartitioning) {
        initializePartitioningVariables(key);
        goPartitioningMainMenuScreen();
        break;
      }
    }
  }
}
