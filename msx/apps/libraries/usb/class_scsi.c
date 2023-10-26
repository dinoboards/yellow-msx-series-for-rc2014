#include "class_scsi.h"
#include "usb_state.h"
#include <stdbool.h>
#include <string.h>
#include <system_vars.h>
#include <z80.h>

_scsi_command_block_wrapper scsi_command_block_wrapper = {{0x55, 0x53, 0x42, 0x43}, {0, 0}, 0, 0, 0, 0};

// lun => B
// cmd_buffer_length => C
// send_receive_buffer_length => DE
// cmd_buffer => HL
// send_receive_buffer => IX
// send => Cy
usb_error
do_scsi_cmd(device_config *const dev, _scsi_command_block_wrapper *const cbw, void *const send_receive_buffer, const bool send) {

  usb_error result;

  EI;
  HALT;
  cbw->dCBWTag[0] = JIFFY;

  if (!send)
    cbw->bmCBWFlags = 0x80;

  CHECK(usb_data_out_transfer((uint8_t *)cbw, sizeof(_scsi_command_block_wrapper) + 16, dev->address,
                              &dev->endpoints[ENDPOINT_BULK_OUT]));

  if (cbw->dCBWDataTransferLength != 0) {
    if (!send) {
      CHECK(usb_data_in_transfer(send_receive_buffer, (uint16_t)cbw->dCBWDataTransferLength, dev->address,
                                 &dev->endpoints[ENDPOINT_BULK_IN]));

    } else {
      CHECK(usb_data_out_transfer(send_receive_buffer, (uint16_t)cbw->dCBWDataTransferLength, dev->address,
                                  &dev->endpoints[ENDPOINT_BULK_OUT]));
    }
  }

  _scsi_command_status_wrapper csw;
  CHECK(
      usb_data_in_transfer((uint8_t *)&csw, sizeof(_scsi_command_status_wrapper), dev->address, &dev->endpoints[ENDPOINT_BULK_IN]));

  if (csw.bCSWStatus != 0 && csw.dCSWTag[0] != cbw->dCBWTag[0])
    return USB_ERR_FAIL;

  return USB_ERR_OK;
}

_scsi_read_capacity scsi_read_capacity = {0x25, 0, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0}};

usb_error get_scsi_read_capacity(device_config *const dev, scsi_read_capacity_result *cap_result) {
  usb_error result;

  cbw_scsi_read_capacity cbw_scsi;
  cbw_scsi.cbw           = scsi_command_block_wrapper;
  cbw_scsi.read_capacity = scsi_read_capacity;

  cbw_scsi.cbw.bCBWLUN                = 0;
  cbw_scsi.cbw.bCBWCBLength           = sizeof(_scsi_read_capacity);
  cbw_scsi.cbw.dCBWDataTransferLength = sizeof(scsi_read_capacity_result);

  CHECK(do_scsi_cmd(dev, &cbw_scsi.cbw, cap_result, false));

  return result;
}

_scsi_packet_inquiry scsi_packet_inquiry = {0x12, 0, 0, 0, 0x24, 0, {0, 0, 0, 0, 0, 0}};

usb_error scsi_inquiry(device_config *const dev, scsi_inquiry_result *inq_result) {
  usb_error result;

  cbw_scsi_inquiry cbw_scsi;
  cbw_scsi.cbw     = scsi_command_block_wrapper;
  cbw_scsi.inquiry = scsi_packet_inquiry;

  cbw_scsi.cbw.bCBWLUN                = 0;
  cbw_scsi.cbw.bCBWCBLength           = sizeof(_scsi_packet_inquiry);
  cbw_scsi.cbw.dCBWDataTransferLength = 0x24;

  CHECK(do_scsi_cmd(dev, &cbw_scsi.cbw, inq_result, false));

  return USB_ERR_OK;
}

usb_error scsi_test(device_config *const dev) {
  cbw_scsi_test cbw_scsi;
  cbw_scsi.cbw = scsi_command_block_wrapper;
  memset(&cbw_scsi.test, 0, sizeof(_scsi_packet_test));

  cbw_scsi.cbw.bCBWLUN                = 0;
  cbw_scsi.cbw.bCBWCBLength           = sizeof(_scsi_packet_test);
  cbw_scsi.cbw.dCBWDataTransferLength = 0;

  return do_scsi_cmd(dev, &cbw_scsi.cbw, 0, false);
}

_scsi_packet_request_sense scsi_packet_request_sense = {0x03, 0, 0, 0, 18, 0, {0, 0, 0, 0, 0, 0}};

usb_error scsi_request_sense(device_config *const dev, scsi_sense_result *const sens_result) {

  cbw_scsi_request_sense cbw_scsi;
  cbw_scsi.cbw           = scsi_command_block_wrapper;
  cbw_scsi.request_sense = scsi_packet_request_sense;

  cbw_scsi.cbw.bCBWLUN                = 0;
  cbw_scsi.cbw.bCBWCBLength           = sizeof(_scsi_packet_request_sense);
  cbw_scsi.cbw.dCBWDataTransferLength = sizeof(scsi_sense_result);

  return do_scsi_cmd(dev, &cbw_scsi.cbw, sens_result, false);
}

usb_error scsi_sense_init(device_config *const dev) {
  scsi_sense_result response;
  usb_error         result;
  uint8_t           counter = 3;

  while ((result = scsi_test(dev)) && --counter > 0)
    scsi_request_sense(dev, &response);

  return result;
}

usb_error scsi_read_write(
    device_config *const dev, const bool send, uint32_t sector_number, const uint8_t sector_count, uint8_t *const buffer) {
  cbw_scsi_read_write cbw;
  memset(&cbw, 0, sizeof(cbw_scsi_read_write));
  cbw.cbw = scsi_command_block_wrapper;

  cbw.cbw.bCBWLUN                = 0;
  cbw.cbw.bCBWCBLength           = sizeof(_scsi_packet_read_write);
  cbw.cbw.dCBWDataTransferLength = 512 * sector_count;

  cbw.scsi_cmd.operation_code  = send ? 0x2A : 0x28;
  cbw.scsi_cmd.transfer_len[1] = sector_count;
  cbw.scsi_cmd.lba[0]          = sector_number >> 24;
  cbw.scsi_cmd.lba[1]          = sector_number >> 16;
  cbw.scsi_cmd.lba[2]          = sector_number >> 8;
  cbw.scsi_cmd.lba[3]          = sector_number;

  return do_scsi_cmd(dev, &cbw.cbw, buffer, send);
}

usb_error scsi_eject(device_config *const dev) {
  cbw_scsi_eject cbw_scsi;
  cbw_scsi.cbw = scsi_command_block_wrapper;

  memset(&cbw_scsi.eject, 0, sizeof(_scsi_packet_eject));

  cbw_scsi.eject.operation_code = 0x1B;
  cbw_scsi.eject.loej           = 1;

  cbw_scsi.cbw.bCBWLUN                = 0;
  cbw_scsi.cbw.bCBWCBLength           = sizeof(_scsi_packet_eject);
  cbw_scsi.cbw.dCBWDataTransferLength = 0;

  return do_scsi_cmd(dev, &cbw_scsi.cbw, 0, false);
}

usb_error scsi_spike(device_config *const dev) {
  cbw_scsi_eject cbw_scsi;
  cbw_scsi.cbw = scsi_command_block_wrapper;

  memset(&cbw_scsi.eject, 0, sizeof(_scsi_packet_eject));

  cbw_scsi.eject.operation_code = 0x1E;
  cbw_scsi.eject.loej           = 0;

  cbw_scsi.cbw.bCBWLUN                = 0;
  cbw_scsi.cbw.bCBWCBLength           = sizeof(_scsi_packet_eject);
  cbw_scsi.cbw.dCBWDataTransferLength = 0;

  return do_scsi_cmd(dev, &cbw_scsi.cbw, 0, false);
}
