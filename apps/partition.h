#ifndef __PARTITION_H
#define __PARTITION_H

#include "datatypes.h"

#define MAX_PARTITIONS_TO_HANDLE                   255
#define MIN_DEVICE_SIZE_IN_K                       10
#define MIN_DEVICE_SIZE_FOR_PARTITIONS_IN_K        1024
#define MIN_REMAINING_SIZE_FOR_NEW_PARTITIONS_IN_K 100
#define MIN_PARTITION_SIZE_IN_K                    100
#define MAX_DEVICE_SIZE_FOR_DIRECT_FORMAT_IN_K     ((uint32_t)32768)
#define MAX_FAT16_PARTITION_SIZE_IN_M              4096
#define MAX_FAT16_PARTITION_SIZE_IN_K              ((uint32_t)MAX_FAT16_PARTITION_SIZE_IN_M * (uint32_t)1024)
#define MAX_FAT12_PARTITION_SIZE_IN_K              (((uint32_t)32768)
#define MAX_SECTORS_PER_TRACK 63
// Each partition has 1 extra sector for MBR
#define EXTRA_PARTITION_SECTORS 1

#define PARTYPE_UNUSED      0
#define PARTYPE_FAT12       1
#define PARTYPE_FAT16_SMALL 4
#define PARTYPE_EXTENDED    5
#define PARTYPE_FAT16       6
#define PARTYPE_FAT16_LBA   0x0E

#define PSTATE_EXISTS  0
#define PSTATE_ADDED   1
#define PSTATE_DELETED 2

#define MAX_FAT12_CLUSTER_COUNT   4084
#define MAX_FAT16_CLUSTER_COUNT   65524
#define FAT12_ROOT_DIR_ENTRIES    112
#define FAT16_ROOT_DIR_ENTRIES    512
#define DIR_ENTRIES_PER_SECTOR    (512 / 32)
#define MAX_FAT12_SECTORS_PER_FAT 12
#define MAX_FAT16_SECTORS_PER_FAT 256
#define FAT_COPIES                2

typedef struct {
  uint8_t  primaryIndex;
  uint8_t  extendedIndex;
  uint8_t  partitionType;
  uint8_t  status;
  uint32_t sizeInK;
  uint16_t alignmentPaddingInSectors;
} partitionInfo;

typedef struct {
  uint32_t totalSectors;
  uint32_t dataSectors;
  uint16_t clusterCount;
  uint16_t sectorsPerFat;
  uint8_t  sectorsPerCluster;
  uint8_t  sectorsPerRootDirectory;
  bool     isFat16;
} dosFilesystemParameters;

typedef struct {
  uint8_t  jumpInstruction[3];
  char     oemNameString[8];
  uint16_t sectorSize;
  uint8_t  sectorsPerCluster;
  uint16_t reservedSectors;
  uint8_t  numberOfFats;
  uint16_t rootDirectoryEntries;
  uint16_t smallSectorCount;
  uint8_t  mediaId;
  uint16_t sectorsPerFat;
  uint16_t sectorsPerTrack;
  uint16_t numberOfHeads;
  union {
    struct {
      uint32_t hiddenSectors;
      uint32_t bigSectorCount;
      uint8_t  physicalDriveNum;
      uint8_t  reserved;
      uint8_t  extendedBlockSignature;
      uint32_t serialNumber;
      char     volumeLabelString[11];
      char     fatTypeString[8];
    } standard;
    struct {
      uint16_t hiddenSectors;
      uint8_t  z80JumpInstruction[2];
      char     volIdString[6];
      uint8_t  dirtyDiskFlag;
      uint32_t serialNumber;
      char     volumeLabelString[11];
      char     fatTypeString[8];
      uint8_t  z80BootCode;
    } DOS220;
  } params;
  uint8_t  emptySpace[447];
  uint16_t mbrSignature;
} fatBootSector;

typedef struct {
  uint8_t  status;
  uint8_t  chsOfFirstSector[3];
  uint8_t  partitionType;
  uint8_t  chsOfLastSector[3];
  uint32_t firstAbsoluteSector;
  uint32_t sectorCount;
} partitionTableEntry;

typedef struct {
  uint8_t             jumpInstruction[3];
  char                oemNameString[8];
  uint8_t             emptySpace[435];
  partitionTableEntry primaryPartitions[4];
  uint16_t            mbrSignature;
} masterBootRecord;

#endif //__PARTITION_H
