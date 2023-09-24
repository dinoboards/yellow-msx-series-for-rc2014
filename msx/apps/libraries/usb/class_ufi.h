#ifndef __CLASS_UFI2
#define __CLASS_UFI2

#include "ch376.h"
#include "protocol.h"
#include "usb_cbi.h"
#include "usb_state.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  uint8_t bASC;
  uint8_t bASCQ;
} ufi_interrupt_status;

typedef struct {
  uint8_t operation_code;
  uint8_t reserverd1 : 5;
  uint8_t lun : 3;
  uint8_t reserved1[5];
  uint8_t allocation_length[2];
  uint8_t reserved[3];
} ufi_read_format_capacities_command;

typedef enum { UNFORMATTED_MEDIA = 1, FORMATTED_MEDIA = 2, NO_MEDIA = 3 } UFI_DESCRIPTOR_CODE;

typedef struct {
  uint8_t reserved1[3];
  uint8_t capacity_list_length;
  uint8_t number_of_blocks[4];
  uint8_t descriptor_code : 2; // UFI_DESCRIPTOR_CODE
  uint8_t reserved2 : 6;
  uint8_t block_size[3];
} ufi_format_capacities_response;

typedef struct {
  uint8_t operation_code;
  uint8_t lun;
  uint8_t reserved[10];
} ufi_test_unit_ready_command;

typedef struct {
  uint8_t operation_code;
  uint8_t evpd : 1;
  uint8_t reserved1 : 4;
  uint8_t lun : 3;
  uint8_t page_code;
  uint8_t reserved3;
  uint8_t allocation_length;
  uint8_t reserved4[7];
} ufi_inquiry_command;

typedef struct {
  uint8_t operation_code;
  uint8_t reserverd1 : 5;
  uint8_t lun : 3;
  uint8_t reserved2;
  uint8_t reserved3;
  uint8_t allocation_length;
  uint8_t reserved4[7];
} ufi_request_sense_command;

typedef struct {
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
} ufi_request_sense_response;

typedef struct {
  // device_type: identifies the device currently connected to the requested logical unit.
  // 00h Direct-access device (floppy)
  // 1Fh none (no FDD connected to the requested logical unit)
  uint8_t device_type : 5;
  uint8_t reserved1 : 3;

  uint8_t reserved2 : 7;
  // Removable Media Bit: this shall be set to one to indicate removable media.
  uint8_t removable_media : 1;

  // ANSI Version: must contain a zero to comply with this version of the Specification.
  uint8_t ansi_version : 3;
  // ISO/ECMA: These fields shall be zero for the UFI device.
  uint8_t ecma : 3;
  uint8_t iso_version : 2;

  // Response Data Format: a value of 01h shall be used for UFI device
  uint8_t response_data_format : 4;
  uint8_t reserved3 : 4;

  // The Additional Length field shall specify the length in bytes of the parameters. If the Allocation Length of the
  // Command Packet is too small to transfer all of the parameters, the Additional Length shall not be adjusted to
  // reflect the truncation. The UFI device shall set this field to 1Fh.
  uint8_t additional_length;
  uint8_t reserved4[3];

  // The Vendor Identification field contains 8 bytes of ASCII data identifying the vendor of the product. The data
  // shall be left aligned within this field.
  char vendor_information[8];

  // The Product Identification field contains 16 bytes of ASCII data as defined by the vendor. The data shall be
  // left-aligned within this field.
  char product_id[16];

  // The Product Revision Level field contains 4 bytes of ASCII data as defined by the vendor.
  char product_revision[4];
} ufi_inquiry_response;

typedef struct {
  uint8_t operation_code; /*0*/
  union {                 /*1*/
    uint8_t byte_1;
    struct {
      uint8_t rel_adr : 1;
      uint8_t reserved1 : 2;
      uint8_t fua : 1;
      uint8_t dpo : 1;
      uint8_t lun : 3;
    };
  };
  uint8_t lba[4];             /*2, 3, 4, 5*/
  uint8_t reserved2;          /*6*/
  uint8_t transfer_length[2]; /*7, 8*/
  uint8_t reserved3[3];       /*9, 10, 11*/
} ufi_read_write_command;

extern usb_error ufi_request_sense(device_config *const storage_device, ufi_request_sense_response const *response);

extern usb_error ufi_read_format_capacities(device_config *const storage_device, ufi_format_capacities_response const *response);

extern usb_error ufi_test_unit_ready(device_config *const storage_device, ufi_request_sense_response const *response);

extern usb_error ufi_inquiry(device_config *const storage_device, ufi_inquiry_response const *response);

extern usb_error ufi_read_write_sector(device_config *const storage_device,
                                       const bool           send,
                                       const uint16_t       sector_number,
                                       const uint8_t        sector_count,
                                       const uint8_t *const buffer);

uint8_t wait_for_device_ready(device_config *const storage_device, const uint16_t timeout_period);

#endif
