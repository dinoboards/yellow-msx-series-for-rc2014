#ifndef __UFI
#define __UFI

#include "work-area.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct _ufi_request_inquiry { // contains information about a specific device
  uint8_t operation_code;
  uint8_t lun;
  uint8_t reserved1;
  uint8_t reserved2;
  uint8_t allocation_length;
  uint8_t reserved3;
  uint8_t pad[6];
} ufi_request_inquiry;

typedef struct _ufi_response_inquiry {
  uint8_t error_code;
  uint8_t reserved1;
  uint8_t sense_key : 4;
  uint8_t reserved2 : 4;
  uint8_t information[4];
  uint8_t additional_length;
  uint8_t reserved3[4];
  uint8_t asc;  // Additional Sense Code
  uint8_t ascq; // Additional Sense Code Qualifier
  uint8_t reserved4[4];
} ufi_response_inquiry;

extern uint8_t ufi_inquiry(_usb_state *const usb_state);

#endif
