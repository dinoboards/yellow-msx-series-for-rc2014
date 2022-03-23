#ifndef __SCSI
#define __SCSI

#include "work-area.h"
#include <stdbool.h>
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

typedef struct __scsi_read_capacity {
  uint8_t operation_code;
  uint8_t lun;
  uint8_t reserved[8];
  uint8_t pad[2];
} _scsi_read_capacity;

typedef struct __scsi_packet_read_write {
  uint8_t operation_code;
  uint8_t lun;
  uint8_t lba[4]; // high-endian block number
  uint8_t reserved1;
  uint8_t transfer_len[2]; // high-endian in blocks of block_len (see scsi_capacity)
  uint8_t reserved2;
  uint8_t pad[2];
} _scsi_packet_read_write;

extern _scsi_command_block_wrapper scsi_command_block_wrapper;
extern _scsi_packet_inquiry        scsi_packet_inquiry;
extern _scsi_packet_test           scsi_packet_test;
extern _scsi_packet_request_sense  scsi_packet_request_sense;
extern _scsi_read_capacity         scsi_read_capacity;
extern _scsi_packet_read_write     scsi_packet_read;
extern _scsi_packet_read_write     scsi_packet_write;

// device_address => a
// lun => B
// cmd_buffer_length => C
// send_receive_buffer_length => DE
// cmd_buffer => HL
// send_receive_buffer => IX
// send => Cy
extern uint8_t do_scsi_cmd(ch376_work_area *const work_area,
                           const uint8_t          device_address,
                           const uint8_t          lun,
                           const uint8_t          cmd_buffer_length,
                           const uint16_t         send_receive_buffer_length,
                           uint8_t *const         cmd_buffer,
                           uint8_t *const         send_receive_buffer,
                           bool                   send);

extern uint8_t do_scsi_cmd_with_reset_retry(ch376_work_area *const work_area,
                           const uint8_t          device_address,
                           const uint8_t          lun,
                           const uint8_t          cmd_buffer_length,
                           const uint16_t         send_receive_buffer_length,
                           uint8_t *const         cmd_buffer,
                           uint8_t *const         send_receive_buffer,
                           bool                   send);

#endif
