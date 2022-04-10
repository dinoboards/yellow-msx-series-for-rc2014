#ifndef __USB_SCSI
#define __USB_SCSI

#include "hw.h"

typedef struct {
  uint8_t  dCBWSignature[4];
  uint8_t  dCBWTag[4];
  uint32_t dCBWDataTransferLength;
  uint8_t  bmCBWFlags;
  uint8_t  bCBWLUN;
  uint8_t  bCBWCBLength;
} _scsi_command_block_wrapper;

typedef struct {
  uint8_t operation_code;
  uint8_t lun;
  uint8_t reserved1;
  uint8_t reserved2;
  uint8_t allocation_length;
  uint8_t reserved3;
  uint8_t pad[6];
} _scsi_packet_request_sense;

typedef struct {
  _scsi_command_block_wrapper cbw;
  _scsi_packet_request_sense  request_sense;
} cbw_scsi_request_sense;

typedef struct {
  uint8_t operation_code;
  uint8_t lun;
  uint8_t reserved1;
  uint8_t reserved2;
  uint8_t reserved3;
  uint8_t reserved4;
  uint8_t pad[6];
} _scsi_packet_test;

typedef struct {
  _scsi_command_block_wrapper cbw;
  _scsi_packet_test           test;
} cbw_scsi_test;

typedef struct {
  uint8_t operation_code;
  uint8_t lun;
  uint8_t reserved[8];
  uint8_t pad[2];
} _scsi_read_capacity;

typedef struct {
  _scsi_command_block_wrapper cbw;
  _scsi_read_capacity         read_capacity;
} cbw_scsi_read_capacity;

typedef struct __scsi_packet_inquiry { // contains information about a specific device
  uint8_t operation_code;
  uint8_t lun;
  uint8_t reserved1;
  uint8_t reserved2;
  uint8_t allocation_length;
  uint8_t reserved3;
  uint8_t pad[6];
} _scsi_packet_inquiry;

typedef struct {
  _scsi_command_block_wrapper cbw;
  _scsi_packet_inquiry        inquiry;
} cbw_scsi_inquiry;

typedef struct {
  uint8_t buffer[95];

  //  UCHAR DeviceType : 5;
  //     UCHAR DeviceTypeQualifier : 3;
  //     UCHAR DeviceTypeModifier : 7;
  //     UCHAR RemovableMedia : 1;
  //     union {
  //         UCHAR Versions;
  //         struct {
  //             UCHAR ANSIVersion : 3;
  //             UCHAR ECMAVersion : 3;
  //             UCHAR ISOVersion : 2;
  //         };
  //     };
  //     UCHAR ResponseDataFormat : 4;
  //     UCHAR HiSupport : 1;
  //     UCHAR NormACA : 1;
  //     UCHAR TerminateTask : 1;
  //     UCHAR AERC : 1;
  //     UCHAR AdditionalLength;
  //     union {
  //         UCHAR Reserved;
  //         struct {
  //             UCHAR PROTECT : 1;
  //             UCHAR Reserved_1 : 2;
  //             UCHAR ThirdPartyCoppy : 1;
  //             UCHAR TPGS : 2;
  //             UCHAR ACC : 1;
  //             UCHAR SCCS : 1;
  //        };
  //     };
  //     UCHAR Addr16 : 1;               // defined only for SIP devices.
  //     UCHAR Addr32 : 1;               // defined only for SIP devices.
  //     UCHAR AckReqQ: 1;               // defined only for SIP devices.
  //     UCHAR MediumChanger : 1;
  //     UCHAR MultiPort : 1;
  //     UCHAR ReservedBit2 : 1;
  //     UCHAR EnclosureServices : 1;
  //     UCHAR ReservedBit3 : 1;
  //     UCHAR SoftReset : 1;
  //     UCHAR CommandQueue : 1;
  //     UCHAR TransferDisable : 1;      // defined only for SIP devices.
  //     UCHAR LinkedCommands : 1;
  //     UCHAR Synchronous : 1;          // defined only for SIP devices.
  //     UCHAR Wide16Bit : 1;            // defined only for SIP devices.
  //     UCHAR Wide32Bit : 1;            // defined only for SIP devices.
  //     UCHAR RelativeAddressing : 1;
  //     UCHAR VendorId[8];
  //     UCHAR ProductId[16];
  //     UCHAR ProductRevisionLevel[4];
  //     UCHAR VendorSpecific[20];
  //     UCHAR Reserved3[2];
  //     VERSION_DESCRIPTOR VersionDescriptors[8];
  //     UCHAR Reserved4[30];

} scsi_inquiry_result;

typedef struct __scsi_command_status_wrapper {
  uint8_t cbwsignature[4];
  uint8_t cbwtag[4];
  uint8_t cbwresidue[4];
  uint8_t cbwstatus;
} _scsi_command_status_wrapper;

typedef struct {
  uint8_t number_of_blocks[4];
  uint8_t block_length[4];
} scsi_read_capacity_result;

// typedef struct __scsi_devinfo {
//   uint8_t                      tag;
//   uint8_t                      max_luns;
//   uint8_t                      buffer[0x24]; // longest response (inquiry) we want to absorb during init
//   _scsi_command_status_wrapper csw;
//   uint8_t                      vendorid[8];
//   uint8_t                      ignored1;
//   uint8_t                      productid[16];
//   uint8_t                      ignored2;
//   uint8_t                      productrev[8];
//   uint8_t                      ignored3;
// } _scsi_devinfo;

extern usb_error get_scsi_read_capacity(scsi_read_capacity_result *result);
extern usb_error get_scsi_max_luns();
extern usb_error get_scsi_inquiry(scsi_inquiry_result *inq_result);
extern usb_error wait_for_mounting();

#endif
