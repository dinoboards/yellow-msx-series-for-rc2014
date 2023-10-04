#ifndef __FDISK2__H
#define __FDISK2__H

#include "datatypes.h"
#include "partition.h"

extern void sectorBootCode();

extern uint8_t createFat12FileSystem(uint8_t driverSlot, uint8_t deviceIndex, uint8_t lunIndex);

extern void clearSectorBuffer();

extern uint16_t deviceSectorRead(uint8_t driverSlot, uint8_t deviceIndex, uint8_t lunIndex, uint32_t sectorNumber);
extern uint16_t deviceSectorWrite(uint8_t driverSlot, uint8_t deviceIndex, uint8_t lunIndex, uint32_t sectorNumber);

extern uint8_t sectorBuffer[512];

#define readSectorFromDevice(driverSlot, deviceIndex, lunIndex, sectorNumber)                                                      \
  deviceSectorRead(driverSlot, deviceIndex, lunIndex, sectorNumber)

#define writeSectorToDevice(driverSlot, deviceIndex, lunIndex, sectorNumber)                                                       \
  deviceSectorWrite(driverSlot, deviceIndex, lunIndex, sectorNumber)

#endif
