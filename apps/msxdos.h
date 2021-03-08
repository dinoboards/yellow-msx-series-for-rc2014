#ifndef __NEXTOR_DOS
#define __NEXTOR_DOS

#include <stdbool.h>
#include <stdlib.h>

#define DRIVER_NAME_LENGTH       32
#define MAX_INSTALLED_DRIVERS    8
#define MAX_LUNS_PER_DEVICE      7
#define MAX_INFO_LENGTH          64
#define MAX_DEVICES_PER_DRIVER   7
#define MAX_ERROR_EXPLAIN_LENGTH 64

#define BLOCK_DEVICE    0
#define READ_ONLY_LUN   (1 << 1)
#define FLOPPY_DISK_LUN (1 << 2)

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
} msxddosLunInfo;

typedef struct {
  uint8_t typeCode;
  uint8_t status;
  union {
    uint32_t startingSector;
    uint32_t partitionSector;
  };
  uint32_t sectorCount;
} GPartInfo;

extern uint8_t msxDosGdrvr(int8_t driverIndex, msxdosDriverInfo *data);
extern uint8_t msxdosGpartInfo(uint8_t slotNumber, uint8_t deviceNumber, uint8_t logicalUnitNumber, uint8_t primaryPartitionNumber, uint8_t extendedPartitionNumber, bool getSectorNumber, GPartInfo *result);
extern uint8_t msxdosExplain(uint8_t code, char *buffer);

extern uint16_t msxdosDrvDevLogicalUnitCount(uint8_t slotNumber, uint8_t deviceNumber, msxdosDeviceBasicInfo *pCount);
extern uint16_t msxdosDrvDevGetName(uint8_t slotNumber, uint8_t deviceNumber, char *pDeviceName);
extern uint16_t msxdosDrvLunInfo(uint8_t slotNumber, uint8_t deviceNumber, uint8_t lunIndex, msxddosLunInfo *pLunInfo);

#define _IPART 0xB4
#define _NOFIL 0xD7

// MSX BIOS functions
void msxbiosInit32();
void msxbiosInitxt();
void msxbiosPosit(uint8_t col, uint8_t row);

/* MSX work area variables */

#define LINL40 0xF3AE
#define LINL32 0xF3AF
#define LINLEN 0xF3B0
#define CRTCNT 0xF3B1
#define CSRY   0xF3DC
#define CSRX   0xF3DD
#define CNSDFG 0xF3DE
#define VALTYP 0xF663
#define DAC    0xF7F6
#define SCRMOD 0xFCAF
#define EXPTBL 0xFCC1

#endif
