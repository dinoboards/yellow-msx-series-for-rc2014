#include "driver_selection.h"
#include "device_selection.h"
#include "key_inputs.h"
#include "screen_control.h"
#include "string_helper.h"
#include "temp.h"
#include <stdint.h>
#include <stdio.h>

msxdosDriverInfo  drivers[MAX_INSTALLED_DRIVERS];
msxdosDriverInfo *selectedDriver;
uint8_t           installedDriversCount;
char              selectedDriverName[50];

void getDriversInformation(void) {
  uint8_t           error         = 0;
  uint8_t           driverIndex   = 1;
  msxdosDriverInfo *currentDriver = &drivers[0];

  installedDriversCount = 0;

  while (error == 0 && driverIndex <= MAX_INSTALLED_DRIVERS) {
    error = msxdosGdrvr(driverIndex, currentDriver);

    if (error == 0 &&
        (currentDriver->flags & (DRIVER_IS_DOS250 | DRIVER_IS_DEVICE_BASED) == (DRIVER_IS_DOS250 | DRIVER_IS_DEVICE_BASED))) {
      installedDriversCount++;
      terminateRightPaddedStringWithZero(currentDriver->driverName, DRIVER_NAME_LENGTH);
      currentDriver++;
    }
    driverIndex++;
  }
}

void showDriverSelectionScreen(void) {
  uint8_t           i;
  char              slot[4];
  char              rev[3];
  msxdosDriverInfo *currentDriver;
  uint8_t           revByte;
  char             *driverName;

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

    printf("\x1BK%d. %s%sv%d.%d%s on slot %s", i + 1, driverName, " ", currentDriver->versionMain, currentDriver->versionSec, rev,
           slot);

    newLine();
    newLine();

    currentDriver++;
  }

  newLine();
  printf("ESC. Exit");

  printStateMessage("Select the device driver");
}

void goDriverSelectionScreen(void) {
  uint8_t key;

  while (true) {
    showDriverSelectionScreen();
    if (installedDriversCount == 0) {
      return;
    }

    while (true) {
      key = waitKey();
      if (key == ESC)
        return;

      key -= '0';
      if (key >= 1 && key <= installedDriversCount) {
        goDeviceSelectionScreen(key);
        break;
      }
    }
  }
}
