#ifndef __NEXTOR_DOS
#define __NEXTOR_DOS

/* must include stdbool before stdio, as stdio include msx.h which will define its own bool if none defined */
#include <stdbool.h>

#include <stdio.h>
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

extern uint8_t msxdosGetEnvironment(const char *name, char *value, const uint8_t buffer_size);
extern uint8_t msxdosSetEnvironment(const char *name, const char *value) __sdcccall(1);

// MSX-DOS 2 File functions

#define IO_RDONLY      0
#define IO_WRONLY      1
#define IO_INHERITABLE 2

extern uint16_t msxdosOpenFile(const char *filename, const uint8_t open_mode);
extern uint16_t msxdosCreateFile(const char *filename, const uint8_t open_mode);
extern uint16_t msxdosReadFile(const uint8_t file_handle, const void *buf, uint16_t nbytes);
extern uint16_t msxdosCloseFile(const uint8_t file_handle) __sdcccall(1);
extern void     msxdosExplainErrorCode(const uint8_t error_code, char *const error_description);

#define _NCOMP 0xFF
#define _WRERR 0xFE
#define _DISK  0xFD
#define _NRDY  0xFC
#define _VERFY 0xFB
#define _DATA  0xFA
#define _RNF   0xF9
#define _WPROT 0xF8
#define _UFORM 0xF7
#define _NDOS  0xF6
#define _WDISK 0xF5
#define _WFILE 0xF4
#define _SEEK  0xF3
#define _IFAT  0xF2
#define _NOUPB 0xF1
#define _IFORM 0xF0
#define _INTER 0xDF
#define _NORAM 0xDE
#define _IBDOS 0xDC
#define _IDRV  0xDB
#define _IFNM  0xDA
#define _IPATH 0xD9
#define _PLONG 0xD8
#define _NOFIL 0xD7
#define _NODIR 0xD6
#define _DRFUL 0xD5
#define _DKFUL 0xD4
#define _DUPF  0xD3
#define _DIRE  0xD2
#define _FILRO 0xD1
#define _DIRNE 0xD0
#define _IATTR 0xCF
#define _DOT   0xCE
#define _SYSX  0xCD
#define _DIRX  0xCC
#define _FILEX 0xCB
#define _FOPEN 0xCA
#define _OV64K 0xC9
#define _FILE  0xC8
#define _EOF   0xC7
#define _ACCV  0xC6
#define _IPROC 0xC5
#define _NHAND 0xC4
#define _IHAND 0xC3
#define _NOPEN 0xC2
#define _IDEV  0xC1
#define _IENV  0xC0
#define _ELONG 0xBF
#define _IDATE 0xBE
#define _ITIME 0xBD
#define _RAMDX 0xBC
#define _NRAMD 0xBB
#define _HDEAD 0xBA
#define _EOL   0xB9
#define _ISBFN 0xB8

#define _IPART 0xB4

// MSX BIOS functions
extern void msxbiosInit32(void);
extern void msxbiosInitxt(void);
extern void _msxbiosPosit(uint16_t col_and_row) __z88dk_fastcall;
#define msxbiosPosit(col, row) _msxbiosPosit(((uint16_t)(col)) * 256 + row)
extern bool msxbiosBreakX(void);
extern void msxbiosInitPalette(void);

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

extern const char __at(0x80) msxdosCommand[127];

#endif
