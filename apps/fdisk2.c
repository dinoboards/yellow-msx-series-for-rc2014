
#include "fdisk.h"
#include "msxdos.h"
#include "partition.h"

static uint8_t        driverSlot;
static uint8_t        deviceIndex;
static uint8_t        selectedLunIndex;
static uint8_t        partitionsCount;
static partitionInfo *partitions;
static uint8_t        sectorsPerTrack;
static uint32_t       nextDeviceSector;
static uint32_t       mainExtendedPartitionSectorCount;
static uint32_t       mainExtendedPartitionFirstSector;
static uint8_t        sectorBuffer[512];

#define readSectorFromDevice(driverSlot, deviceIndex, lunIndex, firstDeviceSector) deviceSectorRW(driverSlot, deviceIndex, lunIndex, firstDeviceSector, 0)
#define writeSectorToDevice(driverSlot, deviceIndex, lunIndex, firstDeviceSector)  deviceSectorRW(driverSlot, deviceIndex, lunIndex, firstDeviceSector, 1)

uint8_t deviceSectorRW(uint8_t driverSlot, uint8_t deviceIndex, uint8_t lunIndex, uint32_t firstDeviceSector, uint8_t write) { return msxdosDevRw(driverSlot, deviceIndex, lunIndex, firstDeviceSector, write, sectorBuffer); }

void preparePartitioningProcess(uint8_t _driverSlot, uint8_t _deviceIndex, uint8_t _selectedLunIndex, uint8_t _partitionsCount, partitionInfo *_partitions, uint8_t _sectorsPerTrack) {
  uint8_t        i;
  partitionInfo *partition = &partitions[1];

  driverSlot = _driverSlot;
  deviceIndex = _deviceIndex;
  selectedLunIndex = _selectedLunIndex;
  partitionsCount = _partitionsCount;
  partitions = _partitions;
  sectorsPerTrack = _sectorsPerTrack;

  nextDeviceSector = 0;
  mainExtendedPartitionSectorCount = 0;
  mainExtendedPartitionFirstSector = 0;

  for (i = 1; i < partitionsCount; i++) {
    mainExtendedPartitionSectorCount += ((&partitions[i])->sizeInK * 2) + 1; //+1 for the MBR
  }
}

uint8_t toggleStatusBit(uint8_t partitionTableEntryIndex, uint32_t partitonTablesector) {
  uint8_t              error;
  masterBootRecord *   mbr = (masterBootRecord *)sectorBuffer;
  partitionTableEntry *entry;

  error = readSectorFromDevice(driverSlot, deviceIndex, selectedLunIndex, partitonTablesector);
  if (error != 0)
    return error;

  entry = &mbr->primaryPartitions[partitionTableEntryIndex];

  entry->status ^= 0x80;

  return writeSectorToDevice(driverSlot, deviceIndex, selectedLunIndex, partitonTablesector);
}
