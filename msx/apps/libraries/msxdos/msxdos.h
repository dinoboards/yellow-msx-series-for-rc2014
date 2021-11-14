#ifndef __NEXTOR_DOS
#define __NEXTOR_DOS

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>

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

typedef struct {
  uint8_t  status;
  uint8_t  typeCode;
  uint32_t sectorCount;
  union {
    uint32_t startingSector;
    uint32_t partitionSector;
  };
} GPartInfo;

extern uint8_t msxdosGdrvr(int8_t driverIndex, msxdosDriverInfo *data);
extern uint8_t msxdosGpart(uint8_t slotNumber, uint8_t deviceNumber, uint8_t logicalUnitNumber, uint8_t primaryPartitionNumber, uint8_t extendedPartitionNumber, bool getSectorNumber, GPartInfo *result);
extern uint8_t msxdosExplain(uint8_t code, char *buffer);
extern uint8_t msxdosDirio(uint8_t code) __z88dk_fastcall;

extern uint16_t msxdosDrvDevLogicalUnitCount(uint8_t slotNumber, uint8_t deviceNumber, msxdosDeviceBasicInfo *pCount);
extern uint16_t msxdosDrvDevGetName(uint8_t slotNumber, uint8_t deviceNumber, char *pDeviceName);
extern uint16_t msxdosDrvLunInfo(uint8_t slotNumber, uint8_t deviceNumber, uint8_t lunIndex, msxdosLunInfo *pLunInfo);
extern uint16_t msxdosDevRead(uint8_t slotNumber, uint8_t deviceNumber, uint8_t lunIndex, uint32_t firstDeviceSector, uint8_t sectorCount, uint8_t *buffer);
extern uint16_t msxdosDevWrite(uint8_t slotNumber, uint8_t deviceNumber, uint8_t lunIndex, uint32_t firstDeviceSector, uint8_t sectorCount, uint8_t *buffer);

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

extern uint8_t HOKVLD;
extern uint8_t EXTBIO[5];

#define RET_INSTRUCTION 0xC9

extern const char __at 0x80 msxdosCommand[127];

#endif
