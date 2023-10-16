
extern void clearInformationArea(void);
void        printStateMessage(char *string);

void printTargetInfo(void);

void printSize(uint32_t sizeInK);

#include <stdbool.h>
void recalculateAutoPartitionSize(bool setToAllSpaceAvailable);

extern uint8_t  partitionsCount;
extern bool     partitionsExistInDisk;
extern bool     canCreatePartitions;
extern bool     canDoDirectFormat;
extern uint32_t unpartitionnedSpaceInSectors;
extern uint8_t  driverSlot;

#include <partition.h>
extern void preparePartitioningProcess(uint8_t        _driverSlot,
                                       uint8_t        _deviceIndex,
                                       uint8_t        _selectedLunIndex,
                                       uint8_t        _partitionsCount,
                                       partitionInfo *_partitions,
                                       uint8_t        _sectorsPerTrack);

void printDosErrorMessage(uint8_t code, char *header);

extern uint8_t toggleStatusBit(uint8_t partitionTableEntryIndex, uint32_t partitonTablesector);

extern char buffer[1024];

extern void testDeviceAccess(void);
extern void testDeviceWriteAccess(void);
extern void testDeviceFullWriteAccess(void);
extern bool writePartitionTable(void);

#define MESSAGE_ROW 9
