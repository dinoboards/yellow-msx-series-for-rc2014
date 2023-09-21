#ifndef __WORK_AREA
#define __WORK_AREA

#include "ch376.h"
#include "class-printer.h"
#include <stdlib.h>

#define MAX_NUMBER_OF_STORAGE_DEVICES 4

typedef struct __usb_state {
  device_config storage_device[MAX_NUMBER_OF_STORAGE_DEVICES];
  device_config hub_config;
  device_config cdc_config;
  device_config printer;
  uint16_t      printer_throughput_limit;
  uint8_t       next_storage_device_index;
  uint8_t       printer_time_throttle_flag;
} _usb_state;

typedef struct _work_area {
  uint8_t    read_count;           // COUNT OF SECTORS TO BE READ
  uint16_t   index;                // sector number to be read
  uint8_t *  dest;                 // destination write address
  uint8_t    read_count_requested; // number of sectors requested
  uint8_t    present;              // BIT FIELD FOR DETECTED DEVICES (BIT 0 -> COMPACTFLASH/IDE, BIT 1-> MSX-MUSIC NOR FLASH)
  _usb_state ch376;
} work_area;

extern work_area * get_work_area();
extern _usb_state *get_usb_work_area();

#endif
