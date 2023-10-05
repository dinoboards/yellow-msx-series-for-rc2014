#ifndef __NEXTOR_DOS
#define __NEXTOR_DOS

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define DRIVER_NAME_LENGTH       32
#define MAX_INSTALLED_DRIVERS    8
#define MAX_LUNS_PER_DEVICE      7
#define MAX_INFO_LENGTH          64
#define MAX_DEVICES_PER_DRIVER   7
#define MAX_ERROR_EXPLAIN_LENGTH 65

#define BLOCK_DEVICE    0
#define READ_ONLY_LUN   (1 << 1)
#define FLOPPY_DISK_LUN (1 << 2)

#define DRIVER_IS_DOS250       (1 << 7)
#define DRIVER_IS_DEVICE_BASED 1

// +4: Driver flags:
//     bit 7: 1 => the driver is a Nextor driver
//            0 => the driver is a MSX-DOS driver
//                 (embedded within a MSX-DOS kernel ROM)
//     bits 6-3: Unused, always zero
//     bit 2: 1 if the driver implements the DRV_CONFIG routine
//     bit 1: Unused, always zero
//     bit 0: 1 => the driver is a device-based driver
//            0 => the driver is a drive-based driver
typedef struct {
  uint8_t slot;
  uint8_t segment;
  uint8_t numDrivesAtBootTime;
  uint8_t firstDriveLetterAtBootTime;
  uint8_t flags;
  uint8_t versionMain;
  uint8_t versionSec;
  uint8_t versionRev;
  char    driverName[DRIVER_NAME_LENGTH];
  uint8_t reserved[64 - DRIVER_NAME_LENGTH - 8];
  uint8_t number;
} msxdosDriverInfo;

typedef struct {
  uint8_t lunCount;
  uint8_t reserved;
} msxdosDeviceBasicInfo;

typedef struct {
  uint8_t  mediumType;
  uint16_t sectorSize;
  uint32_t sectorCount;
  uint8_t  flags;
  uint16_t cylinders;
  uint8_t  heads;
  uint8_t  sectorsPerTrack;
  bool     suitableForPartitioning;
  uint8_t  number;
} msxdosLunInfo;

/* Bit marks for msxdosLunInfo.flags */

/* bit 0: if the medium is removable. */
#define LUN_INFO_FLAG_REMOVABLE 0x01

/* bit 1: if the medium is read only. A medium that can dinamically be write protected or write enabled is not considered  to be
 * read-only.*/
#define LUN_INFO_FLAG_READ_ONLY 0x02

/* bit 2: if the medium is a floppy disk drive.*/
#define LUN_INFO_FLAG_FLOPPY 0x04

/* bit 3: 1 if the logical unit should not be used for automapping */
#define LUN_INFO_FLAG_DISABLE_AUTOMAPPING 0x08

typedef struct {
  uint8_t  status;
  uint8_t  typeCode;
  uint32_t sectorCount;
  union {
    uint32_t startingSector;
    uint32_t partitionSector;
  };
} GPartInfo;

typedef struct {
  uint8_t status;
  union {
    struct {
      uint8_t  letter;
      uint8_t  flags;
      uint8_t  reserved;
      uint8_t  filename[13];
      uint16_t startCluster;
      uint32_t startSector;
    } file;

    struct {
      uint8_t  slot;
      uint8_t  segment;
      uint8_t  number;
      uint8_t  index;
      uint8_t  lun;
      uint32_t firstSectorNumber;
      uint8_t  reserved[54];
    } phyical;
  };

} driveLetterInfo;

/**
 * @brief  Get information about a drive letter (_GDLI, 79h)
 *
 * If a drive larger than the maximum drive number supported by the system is specified, an .IDRV error will be returned. Note that
 * if a drive number is specified which is legal in Nextor, but is currently not assigned to any driver, then no error will be
 * returned, but an empty information block will be returned (the drive status byte should be checked).
 * The "first device sector number" is the absolute device sector number that is treated as the first logical sector for the drive;
 * usually it is either the starting sector of a device partition, or the device absolute sector zero, if the device has no
 * partitions. Note that you can't test this value against zero to check whether the drive is assigned to a block device on a
 * device-based driver or not (use the “drive status” field for this purpose). The "start cluster" and "start sector" fields for
 * mounted files were introduced in Nextor 2.1.1. Currently, they will always contain meaningful information, but in future versions
 * of Nextor this could not be true (because non-FAT filesystems with no concept of "clusters" get supported, or for any other
 * reason) and in these cases the fields will have a value of zero. These fields will also be returned as zero in versions of Nextor
 * older than 2.1.1, therefore application programs using this function call should always verify that the values of these fields
 * are non-zero before using them.
 *
 * @param drive_letter physical drive (0=A:, 1=B:, etc)
 * @param pInfo        pointer to buffer to receive driveLetterInfo structure
 * @return uint8_t
 */
extern uint8_t msxdosGetDriveLetterInfo(const uint8_t drive_letter, const driveLetterInfo *pInfo);

extern uint8_t msxdosGdrvr(int8_t driverIndex, msxdosDriverInfo *data);
extern uint8_t msxdosGpart(uint8_t    slotNumber,
                           uint8_t    deviceNumber,
                           uint8_t    logicalUnitNumber,
                           uint8_t    primaryPartitionNumber,
                           uint8_t    extendedPartitionNumber,
                           bool       getSectorNumber,
                           GPartInfo *result);
extern uint8_t msxdosExplain(uint8_t code, char *buffer);
extern uint8_t msxdosDirio(uint8_t code) __z88dk_fastcall;

extern uint16_t msxdosDrvDevLogicalUnitCount(uint8_t slotNumber, uint8_t deviceNumber, msxdosDeviceBasicInfo *pCount);
extern uint16_t msxdosDrvDevGetName(uint8_t slotNumber, uint8_t deviceNumber, char *pDeviceName);
extern uint16_t msxdosDrvLunInfo(uint8_t slotNumber, uint8_t deviceNumber, uint8_t lunIndex, msxdosLunInfo *pLunInfo);
extern uint16_t msxdosDevRead(
    uint8_t slotNumber, uint8_t deviceNumber, uint8_t lunIndex, uint32_t firstDeviceSector, uint8_t sectorCount, uint8_t *buffer);
extern uint16_t msxdosDevWrite(
    uint8_t slotNumber, uint8_t deviceNumber, uint8_t lunIndex, uint32_t firstDeviceSector, uint8_t sectorCount, uint8_t *buffer);

extern uint8_t msxdosSetDate(uint16_t year, uint8_t month, uint8_t date);
extern uint8_t msxdosSetTime(uint8_t hour, uint8_t minutes, uint8_t seconds, uint8_t centiseconds);
extern void    msxdosGetTime(uint8_t *hour, uint8_t *minutes, uint8_t *seconds);

#define _IPART 0xB4
#define _NOFIL 0xD7

// MSX BIOS functions
extern void msxbiosInit32();
extern void msxbiosInitxt();
extern void _msxbiosPosit(uint16_t col_and_row) __z88dk_fastcall;
#define msxbiosPosit(col, row) _msxbiosPosit(((uint16_t)(col)) * 256 + row)
extern bool msxbiosBreakX();
extern void msxbiosInitPalette();

/* MSX work area variables */

#define LINL32 0xF3AF
#define CRTCNT 0xF3B1
#define CNSDFG 0xF3DE
#define VALTYP 0xF663
#define DAC    0xF7F6
#define SCRMOD 0xFCAF
#define EXPTBL 0xFCC1
#define H_CHPH 0xFDA4

extern uint8_t HOKVLD;
extern uint8_t EXTBIO[5];

#define RET_INSTRUCTION 0xC9

extern const char __at (0x80) msxdosCommand[127];

#endif
