#ifndef __FDISK
#define __FDISK

#include "msxdos.h"
#include <stdbool.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

#define MESSAGE_ROW         9
#define PARTITIONS_PER_PAGE 15

typedef struct {
  uint8_t lunCount;
  char    deviceName[MAX_INFO_LENGTH];
  uint8_t number;
} deviceInfo;

void    terminateRightPaddedString(char *string, uint8_t length);
void    composeSlotString(uint8_t slot, char *destination);
void    printSize(uint32_t sizeInK);
uint8_t getRemainingBy1024String(uint32_t value, char *destination);
// uint8_t     getDiskPartitionsInfo(void);
// const char *getDosErrorMessage(uint8_t code);

typedef struct {
  uint8_t screenMode;
  uint8_t screenWidth;
  bool    functionKeysVisible;
} screenConfiguration;

#define ESC          27
#define CURSOR_RIGHT 28
#define CURSOR_LEFT  29
#define KEY_1        49

#endif
