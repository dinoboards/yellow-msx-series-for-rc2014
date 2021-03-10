
#include "fdisk.h"
#include "msxdos.h"
#include "partition.h"
#include <string.h>

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
static uint8_t        sectorBufferBackup[512];

static void sectorBootCode() {}
static int  getNewSerialNumber() { return 0; }

#define readSectorFromDevice(driverSlot, deviceIndex, lunIndex, firstDeviceSector) deviceSectorRead(driverSlot, deviceIndex, lunIndex, firstDeviceSector)
#define writeSectorToDevice(driverSlot, deviceIndex, lunIndex, firstDeviceSector)  deviceSectorWrite(driverSlot, deviceIndex, lunIndex, firstDeviceSector)

uint8_t deviceSectorRead(uint8_t driverSlot, uint8_t deviceIndex, uint8_t lunIndex, uint32_t firstDeviceSector) { return msxdosDevRead(driverSlot, deviceIndex, lunIndex, firstDeviceSector, 1, sectorBuffer); }
uint8_t deviceSectorWrite(uint8_t driverSlot, uint8_t deviceIndex, uint8_t lunIndex, uint32_t firstDeviceSector) { return msxdosDevWrite(driverSlot, deviceIndex, lunIndex, firstDeviceSector, 1, sectorBuffer); }

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

static void clearSectorBuffer() { memset(sectorBuffer, 0, sizeof(sectorBuffer)); }

static uint8_t calculateFatFileSystemParametersFat12(uint32_t fileSystemSizeInK, dosFilesystemParameters *parameters) {
  // Note: Partitions <=16M are defined to have at most 3 sectors per FAT,
  // so that they can boot DOS 1. This limits the cluster count to 1021.

  uint16_t sectorsPerCluster;
  uint16_t sectorsPerFat;
  uint16_t clusterCount;
  uint32_t dataSectorsCount;
  uint16_t difference;
  uint16_t sectorsPerClusterPower;
  uint16_t maxClusterCount = MAX_FAT12_CLUSTER_COUNT;
  uint16_t maxSectorsPerFat = 12;

  if (fileSystemSizeInK <= (2 * (uint32_t)1024)) {
    sectorsPerClusterPower = 0;
    sectorsPerCluster = 1;
  } else if (fileSystemSizeInK <= (4 * (uint32_t)1024)) {
    sectorsPerClusterPower = 1;
    sectorsPerCluster = 2;
  } else if (fileSystemSizeInK <= (8 * (uint32_t)1024)) {
    sectorsPerClusterPower = 2;
    sectorsPerCluster = 4;
  } else if (fileSystemSizeInK <= (16 * (uint32_t)1024)) {
    sectorsPerClusterPower = 3;
    sectorsPerCluster = 8;
  } else {
    sectorsPerClusterPower = 4;
    sectorsPerCluster = 16;
  }

  if (fileSystemSizeInK <= (16 * (uint32_t)1024)) {
    maxClusterCount = 1021;
    maxSectorsPerFat = 3;
    sectorsPerCluster *= 4;
    sectorsPerClusterPower += 2;
  }

  dataSectorsCount = (fileSystemSizeInK * 2) - (FAT12_ROOT_DIR_ENTRIES / DIR_ENTRIES_PER_SECTOR) - 1;

  clusterCount = dataSectorsCount >> sectorsPerClusterPower;
  sectorsPerFat = ((uint16_t)clusterCount + 2) * 3;

  if ((sectorsPerFat & 0x3FF) == 0) {
    sectorsPerFat >>= 10;
  } else {
    sectorsPerFat >>= 10;
    sectorsPerFat++;
  }

  clusterCount = (dataSectorsCount - FAT_COPIES * sectorsPerFat) >> sectorsPerClusterPower;
  dataSectorsCount = (uint16_t)clusterCount * (uint16_t)sectorsPerCluster;

  if (clusterCount > maxClusterCount) {
    difference = clusterCount - maxClusterCount;
    clusterCount = maxClusterCount;
    sectorsPerFat = maxSectorsPerFat;
    dataSectorsCount -= difference * sectorsPerCluster;
  }

  parameters->totalSectors = dataSectorsCount + 1 + (sectorsPerFat * FAT_COPIES) + (FAT12_ROOT_DIR_ENTRIES / DIR_ENTRIES_PER_SECTOR);
  parameters->dataSectors = dataSectorsCount;
  parameters->clusterCount = clusterCount;
  parameters->sectorsPerFat = sectorsPerFat;
  parameters->sectorsPerCluster = sectorsPerCluster;
  parameters->sectorsPerRootDirectory = (FAT12_ROOT_DIR_ENTRIES / DIR_ENTRIES_PER_SECTOR);
  parameters->isFat16 = false;

  return 0;
}

static uint8_t calculateFatFileSystemParametersFat16(uint32_t fileSystemSizeInK, dosFilesystemParameters *parameters) {
  uint8_t  sectorsPerCluster;
  uint16_t sectorsPerFat;
  uint32_t clusterCount;
  uint32_t dataSectorsCount;
  uint16_t sectorsPerClusterPower;
  uint32_t fileSystemSizeInM = fileSystemSizeInK >> 10;
  uint32_t difference;

  if (fileSystemSizeInM <= (uint32_t)128) {
    sectorsPerClusterPower = 2;
    sectorsPerCluster = 4;
  } else if (fileSystemSizeInM <= (uint32_t)256) {
    sectorsPerClusterPower = 3;
    sectorsPerCluster = 8;
  } else if (fileSystemSizeInM <= (uint32_t)512) {
    sectorsPerClusterPower = 4;
    sectorsPerCluster = 16;
  } else if (fileSystemSizeInM <= (uint32_t)1024) {
    sectorsPerClusterPower = 5;
    sectorsPerCluster = 32;
  } else if (fileSystemSizeInM <= (uint32_t)2048) {
    sectorsPerClusterPower = 6;
    sectorsPerCluster = 64;
  } else {
    sectorsPerClusterPower = 7;
    sectorsPerCluster = 128;
  }

  dataSectorsCount = (fileSystemSizeInK * 2) - (FAT16_ROOT_DIR_ENTRIES / DIR_ENTRIES_PER_SECTOR) - 1;
  clusterCount = dataSectorsCount >> sectorsPerClusterPower;
  sectorsPerFat = (clusterCount + 2) >> 8;

  if (((clusterCount + 2) & 0x3FF) != 0) {
    sectorsPerFat++;
  }

  /// Somehow these 3 lines are causing the device selection menu option to fail! - compiler issue?
  clusterCount = (dataSectorsCount - FAT_COPIES * sectorsPerFat);
  clusterCount >>= sectorsPerClusterPower;
  dataSectorsCount = clusterCount << sectorsPerClusterPower;

  if (clusterCount > MAX_FAT16_CLUSTER_COUNT) {
    difference = clusterCount - MAX_FAT16_CLUSTER_COUNT;
    clusterCount = MAX_FAT16_CLUSTER_COUNT;
    sectorsPerFat = 256;
    dataSectorsCount -= difference << sectorsPerClusterPower;
  }

  parameters->totalSectors = dataSectorsCount + 1 + (sectorsPerFat * FAT_COPIES) + (FAT16_ROOT_DIR_ENTRIES / DIR_ENTRIES_PER_SECTOR);
  parameters->dataSectors = dataSectorsCount;
  parameters->clusterCount = clusterCount;
  parameters->sectorsPerFat = sectorsPerFat;
  parameters->sectorsPerCluster = sectorsPerCluster;
  parameters->sectorsPerRootDirectory = (FAT16_ROOT_DIR_ENTRIES / DIR_ENTRIES_PER_SECTOR);
  parameters->isFat16 = true;

  return 0;
}

static void calculateFatFileSystemParameters(uint32_t fileSystemSizeInK, dosFilesystemParameters *parameters) {
  if (fileSystemSizeInK > MAX_FAT12_PARTITION_SIZE_IN_K)
    calculateFatFileSystemParametersFat16(fileSystemSizeInK, parameters);

  else
    calculateFatFileSystemParametersFat12(fileSystemSizeInK, parameters);
}

static void createFatBootSector(dosFilesystemParameters *parameters) {
  fatBootSector *sector = (fatBootSector *)sectorBuffer;

  clearSectorBuffer();

  sector->jumpInstruction[0] = 0xEB;
  sector->jumpInstruction[1] = 0xFE;
  sector->jumpInstruction[2] = 0x90;
  strcpy(sector->oemNameString, "NEXTOR20");
  sector->sectorSize = 512;
  sector->sectorsPerCluster = parameters->sectorsPerCluster;
  sector->reservedSectors = 1;
  sector->numberOfFats = FAT_COPIES;
  sector->rootDirectoryEntries = parameters->sectorsPerRootDirectory * DIR_ENTRIES_PER_SECTOR;
  if ((parameters->totalSectors & 0xFFFF0000) == 0) {
    sector->smallSectorCount = parameters->totalSectors;
  }
  sector->mediaId = 0xF0;
  sector->sectorsPerFat = parameters->sectorsPerFat;
  strcpy(sector->params.standard.volumeLabelString, "NEXTOR 2.0 "); // it is same for DOS 2.20 format
  sector->params.standard.serialNumber = getNewSerialNumber();      // it is same for DOS 2.20 format
  sector->mbrSignature = 0xAA55;

  if (parameters->isFat16) {
    sector->params.standard.bigSectorCount = parameters->totalSectors;
    sector->params.standard.extendedBlockSignature = 0x29;
    strcpy(sector->params.standard.fatTypeString, "FAT16   ");
  } else {
    sector->params.DOS220.z80JumpInstruction[0] = 0x18;
    sector->params.DOS220.z80JumpInstruction[1] = 0x1E;
    strcpy(sector->params.DOS220.volIdString, "VOL_ID");
    strcpy(sector->params.DOS220.fatTypeString, "FAT12   ");
    memcpy(&(sector->params.DOS220.z80BootCode), sectorBootCode, (uint16_t)0xC090 - (uint16_t)0xC03E);
  }
}

static uint8_t createFatFileSystem(uint8_t driverSlot, uint8_t deviceIndex, uint8_t lunIndex, uint32_t firstDeviceSector, uint32_t fileSystemSizeInK) {
  dosFilesystemParameters parameters;
  uint8_t                 error;
  uint32_t                sectorNumber;
  uint16_t                zeroSectorsToWrite;
  uint16_t                i;

  calculateFatFileSystemParameters(fileSystemSizeInK, &parameters);

  //* Boot sector

  createFatBootSector(&parameters);

  if ((error = writeSectorToDevice(driverSlot, deviceIndex, lunIndex, firstDeviceSector)) != 0)
    return error;

  //* FAT (except 1st sector) and root directory sectors

  clearSectorBuffer();
  zeroSectorsToWrite = (parameters.sectorsPerFat * FAT_COPIES) + (parameters.sectorsPerRootDirectory) - 1;
  sectorNumber = firstDeviceSector + 2;
  for (i = 0; i < zeroSectorsToWrite; i++) {
    if ((error = writeSectorToDevice(driverSlot, deviceIndex, lunIndex, sectorNumber)) != 0)
      return error;
    sectorNumber++;
  }

  //* First sector of each FAT

  sectorBuffer[0] = 0xF0;
  sectorBuffer[1] = 0xFF;
  sectorBuffer[2] = 0xFF;
  if (parameters.isFat16) {
    sectorBuffer[3] = 0xFF;
  }
  if ((error = writeSectorToDevice(driverSlot, deviceIndex, lunIndex, firstDeviceSector + 1)) != 0)
    return error;

  if ((error = writeSectorToDevice(driverSlot, deviceIndex, lunIndex, firstDeviceSector + 1 + parameters.sectorsPerFat)) != 0)
    return error;

  return 0;
}

uint8_t createPartition(uint8_t index) {
  uint8_t              error;
  masterBootRecord *   mbr = (masterBootRecord *)sectorBuffer;
  partitionInfo *      partition = &partitions[index];
  uint32_t             mbrSector;
  uint32_t             firstFileSystemSector;
  partitionTableEntry *tableEntry;
  uint32_t             x;

  mbrSector = nextDeviceSector;
  tableEntry = &(mbr->primaryPartitions[0]);
  clearSectorBuffer();
  tableEntry->firstAbsoluteSector = 1;

  tableEntry->status = partition->status;
  tableEntry->partitionType = partition->partitionType;
  tableEntry->sectorCount = partition->sizeInK * 2;

  firstFileSystemSector = mbrSector + tableEntry->firstAbsoluteSector;

  nextDeviceSector += tableEntry->firstAbsoluteSector + tableEntry->sectorCount;

  if (index != (partitionsCount - 1)) {
    tableEntry++;
    tableEntry->partitionType = PARTYPE_EXTENDED;
    tableEntry->firstAbsoluteSector = nextDeviceSector;
    if (index == 0) {
      mainExtendedPartitionFirstSector = nextDeviceSector;
      tableEntry->sectorCount = mainExtendedPartitionSectorCount;
    } else {
      tableEntry->firstAbsoluteSector -= mainExtendedPartitionFirstSector;
      tableEntry->sectorCount = (((partitionInfo *)(partition + 1))->sizeInK * 2);
    }
  }

  if (index == 0) {
    mbr->jumpInstruction[0] = 0xEB;
    mbr->jumpInstruction[1] = 0xFE;
    mbr->jumpInstruction[2] = 0x90;
    strcpy(mbr->oemNameString, "NEXTOR20");
  }

  mbr->mbrSignature = 0xAA55;

  //??????????? WAS THIS SOME WEIRD COMPILER BUG?
  x = tableEntry->firstAbsoluteSector; // Without this, firstAbsoluteSector is written to disk as 0. WTF???
  tableEntry->firstAbsoluteSector = x;

  memcpy(sectorBufferBackup, sectorBuffer, 512);

  if ((error = writeSectorToDevice(driverSlot, deviceIndex, selectedLunIndex, mbrSector)) != 0)
    return error;

  return createFatFileSystem(driverSlot, deviceIndex, selectedLunIndex, firstFileSystemSector, partition->sizeInK);
}
