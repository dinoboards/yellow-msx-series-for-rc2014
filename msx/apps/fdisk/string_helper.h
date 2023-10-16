#ifndef __STRING_HELPER_H
#define __STRING_HELPER_H

#include "stdint.h"

extern void terminateRightPaddedStringWithZero(char *string, uint8_t length);
extern void composeSlotString(uint8_t slot, char *destination);

#endif
