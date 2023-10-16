#ifndef DEVICE_SELECTION_H
#define DEVICE_SELECTION_H

#include <msxdos.h>
#include <stdint.h>

typedef struct {
  uint8_t lunCount;
  char    deviceName[MAX_INFO_LENGTH];
  uint8_t number;
} deviceInfo;

extern deviceInfo *currentDevice;
extern deviceInfo  devices[MAX_DEVICES_PER_DRIVER];
extern uint8_t     selectedDeviceIndex;
extern uint8_t     deviceIndex;

void goDeviceSelectionScreen(uint8_t driverIndex);
void printDeviceInfoWithIndex(void);

#endif
