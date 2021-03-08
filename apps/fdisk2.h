#ifndef __FDISK2
#define __FDISK2

#include "datatypes.h"
#include "partition.h"

extern void    preparePartitioningProcess(uint8_t _driverSlot, uint8_t _deviceIndex, uint8_t _selectedLunIndex, uint8_t _partitionsCount, partitionInfo *_partitions, uint8_t _sectorsPerTrack);
extern uint8_t toggleStatusBit(uint8_t partitionTableEntryIndex, uint32_t partitonTablesector);

#endif
