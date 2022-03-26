#ifndef __WORK_AREA
#define __WORK_AREA

#include <stdlib.h>

// ; USB error codes, these are the ones returned by the
// ; USB routines that are documented as "Output: USB Error code"
#define USB_ERR_OK                          0
#define USB_ERR_NAK                         1
#define USB_ERR_STALL                       2
#define USB_ERR_TIMEOUT                     3
#define USB_ERR_DATA_ERROR                  4
#define USB_ERR_NO_DEVICE                   5
#define USB_ERR_PANIC_BUTTON_PRESSED        6
#define USB_ERR_UNEXPECTED_STATUS_FROM_HOST 7
#define USB_ERR_MAX                         7
#define USB_FILERR_MIN                      0x41
#define USB_ERR_OPEN_DIR                    0x41
#define USB_ERR_MISS_FILE                   0x42
#define USB_FILERR_MAX                      0x0B4

#define DEVICE_ADDRESS 1

typedef enum { USB_IS_FLOPPY = 1, USB_IS_MASS_STORAGE = 2 } usb_device_type;

typedef enum { ENDPOINT_BULK_OUT = 0, ENDPOINT_BULK_IN = 1, ENDPOINT_INTERRUPT_IN = 2 };

typedef struct _endpoint_param {
  uint8_t number;
  uint8_t toggle;
  uint8_t max_packet_size;
} endpoint_param;

typedef struct __usb_state {
  endpoint_param  endpoints[3];
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

extern work_area *get_work_area();

#endif
