#include "string_helper.h"

void terminateRightPaddedStringWithZero(char *string, uint8_t length) {
  char *pointer = string + length - 1;
  while (*pointer == ' ' && length > 0) {
    pointer--;
    length--;
  }
  pointer[1] = '\0';
}

void composeSlotString(uint8_t slot, char *destination) {
  if ((slot & 0x80) == 0) {
    destination[0] = slot + '0';
    destination[1] = '\0';
  } else {
    destination[0] = (slot & 3) + '0';
    destination[1] = '-';
    destination[2] = ((slot >> 2) & 3) + '0';
    destination[3] = '\0';
  }
}
