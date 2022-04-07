#ifndef __WORK_AREA
#define __WORK_AREA

#include "ch376.h"
#include <stdlib.h>

#define DEVICE_ADDRESS_FLOPPY 2
#define DEVICE_ADDRESS_HUB 1

typedef enum { USB_IS_HUB = 1, USB_IS_FLOPPY = 2, USB_IS_MASS_STORAGE = 4 } usb_device_type;

typedef enum { ENDPOINT_BULK_OUT = 0, ENDPOINT_BULK_IN = 1, ENDPOINT_INTERRUPT_IN = 2 } usb_endpoint_type;

typedef struct __usb_state {
  endpoint_param  endpoints[3]; //floppy endpoints
  endpoint_param  hub_endpoint;
  usb_device_type usb_device;
  uint8_t         max_packet_size;
  uint8_t         bConfigurationvalue;
  uint8_t         interface_number;
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
