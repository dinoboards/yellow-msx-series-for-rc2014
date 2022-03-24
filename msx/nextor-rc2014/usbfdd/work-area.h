#ifndef __WORK_AREA
#define __WORK_AREA

#include <stdlib.h>

typedef enum { USB_IS_FLOPPY = 1, USB_IS_MASS_STORAGE = 2 } usb_device_type;

typedef struct _endpoint_param {
  uint8_t number;
  uint8_t toggle;
  uint8_t max_packet_size;
} endpoint_param;

typedef struct _ch376_work_area {
  usb_device_type usb_device;
  endpoint_param  bulk_out;
  endpoint_param  bulk_in;
  endpoint_param  interrupt_in;
} ch376_work_area;

typedef struct _work_area {
  uint8_t         read_count;           // COUNT OF SECTORS TO BE READ
  uint16_t        index;                // sector number to be read
  uint8_t *       dest;                 // destination write address
  uint8_t         read_count_requested; // number of sectors requested
  uint8_t         present;              // BIT FIELD FOR DETECTED DEVICES (BIT 0 -> COMPACTFLASH/IDE, BIT 1-> MSX-MUSIC NOR FLASH)
  ch376_work_area ch376;
} work_area;

extern work_area *get_work_area();

#endif
