
#include "format_fats.h"
#include "read_and_write_sectors.h"
#include "rotator.h"
#include <fat.h>
#include <msxdos.h>
#include <stdio.h>
#include <string.h>
#include <system_vars.h>

extern void sectorBootCode(void);

#define BUFFER_SEC_COUNT 18
#define BUFFER_SIZE      (512 * BUFFER_SEC_COUNT)

#define min(a, b) ((a) < (b) ? (a) : (b))

static int getNewSerialNumber(void) { return 0; }

static void create_fat_12_boot_sector(uint8_t *const buffer, const bool isHD) __sdcccall(1) {
  fatBootSector *sector = (fatBootSector *)buffer;

  memset(buffer, 0, 512);

  sector->jumpInstruction[0] = 0xEB;
  sector->jumpInstruction[1] = 0xFE;
  sector->jumpInstruction[2] = 0x90;
  strcpy(sector->oemNameString, "MSXDOS");
  sector->sectorSize           = 512;
  sector->sectorsPerCluster    = 1;
  sector->reservedSectors      = 1;
  sector->numberOfFats         = FAT_COPIES;
  sector->rootDirectoryEntries = isHD ? 224 : 112;   // 112 for 720k
  sector->smallSectorCount     = isHD ? 2880 : 1440; // 1440 for 720k
  sector->mediaId              = isHD ? 0xF0 : 0xF9; // f9 for 720k
  sector->sectorsPerFat        = isHD ? 9 : 3;       // 3 for 720k
  sector->sectorsPerTrack      = isHD ? 18 : 9;      // 9 for 720k
  sector->numberOfHeads        = 2;
  strcpy(sector->params.standard.volumeLabelString, "XYZ ");                   // it is same for DOS 2.20 format
  sector->params.standard.serialNumber        = (uint32_t)JIFFY << 16 | JIFFY; // it is same for DOS 2.20 format
  sector->mbrSignature                        = 0xAA55;
  sector->params.DOS220.z80JumpInstruction[0] = 0x18; // JUMP 0x1E
  sector->params.DOS220.z80JumpInstruction[1] = 0x1E; // MSXDOS as JUMP 0x10
  strcpy(sector->params.DOS220.volIdString, "VOL_ID");
  strcpy(sector->params.DOS220.fatTypeString, "FAT12   ");
  memcpy(&(sector->params.DOS220.z80BootCode), sectorBootCode, (uint16_t)0xC090 - (uint16_t)0xC03E);
}

uint8_t create_fat_12_file_system(device_config *const storage_device, const bool isHD) __sdcccall(1) {
  uint8_t              buffer[BUFFER_SIZE];
  const fatBootSector *sector = (fatBootSector *)buffer;

  uint8_t  error;
  uint32_t sector_number;
  uint16_t zero_sectors_to_write;

  create_fat_12_boot_sector(buffer, isHD);
  const uint16_t sectorsPerFat        = sector->sectorsPerFat;
  const uint16_t rootDirectoryEntries = sector->rootDirectoryEntries;

  printf("Initializing Boot sector: %3d", 0, 0);
  if ((error = write_sector(storage_device, 0, 1, buffer)) != 0) {
    printf("\r\nwrite_sector failed with error %d\r\n", error);
    return error;
  }
  erase_line();
  printf("\rInitialized Boot sector\r\n");

  memset(buffer, 0, sizeof(buffer));

  zero_sectors_to_write = (sectorsPerFat * FAT_COPIES) + rootDirectoryEntries - 1;
  sector_number         = 1;

  while (sector_number <= zero_sectors_to_write) {
    if (msxbiosBreakX()) {
      printf("\r\n Aborted\r\n");
      return 1;
    }

    const int16_t remaining = min(zero_sectors_to_write - (sector_number - 1), BUFFER_SEC_COUNT);
    printf("\rClearing FAT sectors %3ld of %3d", sector_number, zero_sectors_to_write);
    if ((error = write_sector(storage_device, sector_number, remaining, buffer)) != 0)
      return error;

    sector_number += remaining;
  }

  erase_line();
  printf("\rCleared %3d FAT sectors\r\n", zero_sectors_to_write);

  // First sector of each FAT
  buffer[0] = 0xF0;
  buffer[1] = 0xFF;
  buffer[2] = 0xFF;

  printf("Tagging first FAT @ sector %d\r\n", 1);
  if ((error = write_sector(storage_device, 1, 1, buffer)) != 0)
    return error;

  printf("Tagging second FAT @ sector %d\r\n", sectorsPerFat + 1);
  if ((error = write_sector(storage_device, sectorsPerFat + 1, 1, buffer)) != 0)
    return error;

  return 0;
}

usb_error format_fat(device_config *const storage_device, const disk_geometry *const geometry) __sdcccall(1) {

  const bool isHD = geometry->number_of_blocks == 2880;
  const bool isDD = geometry->number_of_blocks == 1440;
  if (!isHD && !isDD) {
    printf("Unexpceted sector count for device. Expected 2880 or 1440, got %ld\r\nOnly 1.44Mb floppies are supported. Unable to "
           "proceed.\r\n",
           geometry->number_of_blocks);
    return 255;
  }

  return create_fat_12_file_system(storage_device, isHD);
}
