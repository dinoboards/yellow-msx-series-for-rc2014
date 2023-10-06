#ifndef __WORK_AREA
#define __WORK_AREA

#include <ch376.h>
#include <protocol.h>
#include <stdlib.h>
#include <usb_state.h>

typedef struct _work_area {
  uint8_t    read_count;           // COUNT OF SECTORS TO BE READ
  uint16_t   index;                // sector number to be read
  uint8_t   *dest;                 // destination write address
  uint8_t    read_count_requested; // number of sectors requested
  uint8_t    present;              // BIT FIELD FOR DETECTED DEVICES (BIT 0 -> COMPACTFLASH/IDE, BIT 1-> MSX-MUSIC NOR FLASH)
  _usb_state ch376;
} work_area;

extern work_area *get_work_area();

#endif
