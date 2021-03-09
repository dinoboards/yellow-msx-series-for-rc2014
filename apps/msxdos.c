#include "msxdos.h"
#include <stdarg.h>

uint8_t *pMsxdosSafeWorkingBuffer;

uint16_t safeMsxdosDrvDevLogicalUnitCount(uint8_t slotNumber, uint8_t deviceNumber, msxdosDeviceBasicInfo *pCount) {
  const uint16_t error = msxdosDrvDevLogicalUnitCount(slotNumber, deviceNumber, (msxdosDeviceBasicInfo *)pMsxdosSafeWorkingBuffer);

  if (error != 0)
    return error;

  memcpy(pCount, pMsxdosSafeWorkingBuffer, sizeof(msxdosDeviceBasicInfo));

  return 0;
}

uint16_t safeMsxdosDrvDevGetName(uint8_t slotNumber, uint8_t deviceNumber, char *pDeviceName) {

  const uint16_t error = msxdosDrvDevGetName(slotNumber, deviceNumber, (char *)pMsxdosSafeWorkingBuffer);

  if (error != 0)
    return error;

  memcpy(pDeviceName, pMsxdosSafeWorkingBuffer, MAX_INFO_LENGTH);

  return 0;
}

uint16_t safeMsxdosDrvLunInfo(uint8_t slotNumber, uint8_t deviceNumber, uint8_t lunIndex, msxdosLunInfo *pLunInfo) {
  const uint16_t error = msxdosDrvLunInfo(slotNumber, deviceNumber, lunIndex, (msxdosLunInfo *)pMsxdosSafeWorkingBuffer);

  if (error != 0)
    return error;

  memcpy(pLunInfo, pMsxdosSafeWorkingBuffer, sizeof(msxdosLunInfo));

  return 0;
}
