#ifndef __FAT_H
#define __FAT_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_FAT12_CLUSTER_COUNT   4084
#define MAX_FAT16_CLUSTER_COUNT   65524
#define FAT12_ROOT_DIR_ENTRIES    112
#define FAT16_ROOT_DIR_ENTRIES    512
#define DIR_ENTRIES_PER_SECTOR    (512 / 32)
#define MAX_FAT12_SECTORS_PER_FAT 12
#define MAX_FAT16_SECTORS_PER_FAT 256
#define FAT_COPIES                2

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

#endif
