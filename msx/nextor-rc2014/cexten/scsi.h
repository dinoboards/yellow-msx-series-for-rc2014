#ifndef __SCSI
#define __SCSI

#include <stdlib.h>

typedef struct __scsi_command_block_wrapper {
  uint8_t  dCBWSignature[4];
  uint8_t  dCBWTag[4];
  uint32_t dCBWDataTransferLength;
  uint8_t  bmCBWFlags;
  uint8_t  bCBWLUN;
  uint8_t  bCBWCBLength;
} _scsi_command_block_wrapper;

typedef struct __scsi_packet_inquiry { // contains information about a specific device
  uint8_t operation_code;
  uint8_t lun;
  uint8_t reserved1;
  uint8_t reserved2;
  uint8_t allocation_length;
  uint8_t reserved3;
  uint8_t pad[6];
} _scsi_packet_inquiry;

typedef struct __scsi_packet_test {
  uint8_t operation_code;
  uint8_t lun;
  uint8_t reserved1;
  uint8_t reserved2;
  uint8_t reserved3;
  uint8_t reserved4;
  uint8_t pad[6];
} _scsi_packet_test;

typedef struct _scsi_packet_request_sense {
  uint8_t operation_code;
  uint8_t lun;
  uint8_t reserved1;
  uint8_t reserved2;
  uint8_t allocation_length;
  uint8_t reserved3;
  uint8_t pad[6];
} _scsi_packet_request_sense;

extern _scsi_command_block_wrapper scsi_command_block_wrapper;
extern _scsi_packet_inquiry        scsi_packet_inquiry;
extern _scsi_packet_test           scsi_packet_test;
extern _scsi_packet_request_sense  scsi_packet_request_sense;
#endif
