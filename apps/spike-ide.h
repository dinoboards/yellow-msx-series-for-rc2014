#ifndef __SPIKE_IDE
#define __SPIKE_IDE

#include <stdlib.h>

typedef struct {
  uint8_t lba0;
  uint8_t lba1;
  uint8_t lba2;
  uint8_t lba3;
  uint8_t data[1024];
} ioBuf;

extern void cfInit();
extern void cfReadTest(ioBuf*) __z88dk_fastcall;
extern void cfWriteTest(ioBuf*) __z88dk_fastcall;
extern void cfReadIdentity(uint8_t*) __z88dk_fastcall;

//name in ASCII (40 chars @ byte offset $36) and the capacity of the drive in sectors (2 words @ byte offset $72, least significant first).

typedef struct {
  struct {
    uint16_t Reserved1 : 1;
    uint16_t Retired3 : 1;
    uint16_t ResponseIncomplete : 1;
    uint16_t Retired2 : 3;
    uint16_t FixedDevice : 1;
    uint16_t RemovableMedia : 1;
    uint16_t Retired1 : 7;
    uint16_t DeviceType : 1;
  } GeneralConfiguration;
  uint16_t NumCylinders;
  uint16_t SpecificConfiguration;
  uint16_t NumHeads;
  uint16_t Retired1[2];
  uint16_t NumSectorsPerTrack;
  uint16_t VendorUnique1[3];
  char  SerialNumber[20];
  uint16_t Retired2[2];
  uint16_t Obsolete1;
  char  FirmwareRevision[8];
  char  ModelNumber[40];
  char  MaximumBlockTransfer;
  char  VendorUnique2;
  struct {
    // uint16_t FeatureSupported : 1;
    uint16_t Reserved : 15;
  } TrustedComputing;
  struct {
    char  CurrentLongPhysicalSectorAlignment : 2;
    char  ReservedByte49 : 6;
    char  DmaSupported : 1;
    char  LbaSupported : 1;
    char  IordyDisable : 1;
    char  IordySupported : 1;
    char  Reserved1 : 1;
    char  StandybyTimerSupport : 1;
    char  Reserved2 : 2;
    uint16_t ReservedWord50;
  } Capabilities;
  uint16_t ObsoleteWords51[2];
  uint16_t TranslationFieldsValid : 3;
  uint16_t Reserved3 : 5;
  uint16_t FreeFallControlSensitivity : 8;
  uint16_t NumberOfCurrentCylinders;
  uint16_t NumberOfCurrentHeads;
  uint16_t CurrentSectorsPerTrack;
  uint32_t  CurrentSectorCapacity;
} IdeIdentity;

#endif
