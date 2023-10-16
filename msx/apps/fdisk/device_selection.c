#include "device_selection.h"
#include "driver_selection.h"
#include "key_inputs.h"
#include "lun_selection.h"
#include "screen_control.h"
#include "string_helper.h"
#include "temp.h"
#include <msxdos.h>
#include <stdio.h>

deviceInfo  devices[MAX_DEVICES_PER_DRIVER];
static bool availableDevicesCount;
deviceInfo *currentDevice;
uint8_t     selectedDeviceIndex;
uint8_t     deviceIndex;

void printDeviceInfoWithIndex(void) { printf(" (Id = %d)", selectedDeviceIndex); }

void getDevicesInformation(void) {
  uint16_t    error         = 0;
  uint8_t     deviceIndex   = 1;
  deviceInfo *currentDevice = &devices[0];

  availableDevicesCount = 0;

  while (deviceIndex <= MAX_DEVICES_PER_DRIVER) {

    error = msxdosDrvDevLogicalUnitCount(selectedDriver->slot, deviceIndex, (msxdosDeviceBasicInfo *)currentDevice);
    if (error == 0) {
      char *const currentDeviceName = currentDevice->deviceName;
      availableDevicesCount++;
      error = msxdosDrvDevGetName(selectedDriver->slot, deviceIndex, currentDeviceName);

      if (error == 0)
        terminateRightPaddedStringWithZero(currentDeviceName, MAX_INFO_LENGTH);
      else
        sprintf(currentDeviceName, "(Unnamed device, ID=%d)", deviceIndex);
    } else
      currentDevice->lunCount = 0;

    deviceIndex++;
    currentDevice++;
  }
}

void showDeviceSelectionScreen(void) {
  deviceInfo *currentDevice;
  uint8_t     i;

  clearInformationArea();
  locate(0, 3);
  printf(selectedDriverName);
  cursorDown();
  cursorDown();

  if (availableDevicesCount == 0) {
    getDevicesInformation();
  }

  if (availableDevicesCount == 0) {
    locate(0, 9);
    printCentered("There are no suitable devices");
    cursorDown();
    printCentered("attached to the driver");
    printStateMessage("Press any key to go back...");
    waitKey();
    return;
  }

  currentDevice = &devices[0];
  for (i = 0; i < MAX_DEVICES_PER_DRIVER; i++) {
    if (currentDevice->lunCount > 0) {
      printf("\x1BK%d. %s\r\n\r\n", i + 1, currentDevice->deviceName);
    }

    currentDevice++;
  }

  if (availableDevicesCount < 7) {
    newLine();
  }
  printf("ESC. Go back to driver selection screen");

  printStateMessage("Select the device");
}

void goDeviceSelectionScreen(uint8_t driverIndex) {
  char    slot[4];
  uint8_t key;

  selectedDriver = &drivers[driverIndex - 1];
  composeSlotString(selectedDriver->slot, slot);
  strcpy(selectedDriverName, selectedDriver->driverName);
  sprintf(selectedDriverName + strlen(selectedDriverName), " on slot %s\r\n", slot);

  availableDevicesCount = 0;

  while (true) {
    showDeviceSelectionScreen();
    if (availableDevicesCount == 0) {
      return;
    }

    while (true) {
      key = waitKey();
      if (key == ESC)
        return;

      key -= '0';
      if (key >= 1 && key <= MAX_DEVICES_PER_DRIVER && devices[key - 1].lunCount != 0) {
        goLunSelectionScreen(key);
        break;
      }
    }
  }
}
