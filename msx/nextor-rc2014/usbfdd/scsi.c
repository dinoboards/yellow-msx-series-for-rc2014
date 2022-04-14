#include "scsi.h"
#include "work-area.h"
#include <stdbool.h>
#include <string.h>

#include "print.h"

_scsi_command_block_wrapper scsi_command_block_wrapper = {{0x55, 0x53, 0x42, 0x43}, {0, 0, 0, 0}, 0, 0, 0, 0};

// lun => B
// cmd_buffer_length => C
// send_receive_buffer_length => DE
// cmd_buffer => HL
// send_receive_buffer => IX
// send => Cy
usb_error do_scsi_cmd(_usb_state *const                  usb_state,
                      _scsi_command_block_wrapper *const cbw,
                      void *const                        send_receive_buffer,
                      const bool                         send) {

  usb_error result;

  if (!send)
    cbw->bmCBWFlags = 0x80;

  CHECK(hw_data_out_transfer((uint8_t *)cbw,
                             sizeof(_scsi_command_block_wrapper) + 16,
                             DEVICE_ADDRESS_MASS,
                             &usb_state->storage_device[0].endpoints[ENDPOINT_BULK_OUT]),
        x_printf("Err6 %d ", result));

  if (cbw->dCBWDataTransferLength != 0) {
    if (!send) {
      CHECK(hw_data_in_transfer(send_receive_buffer,
                                (uint16_t)cbw->dCBWDataTransferLength,
                                DEVICE_ADDRESS_MASS,
                                &usb_state->storage_device[0].endpoints[ENDPOINT_BULK_IN]),
            x_printf("Err7 %d ", result));

    } else {
      CHECK(hw_data_out_transfer(send_receive_buffer,
                                 (uint16_t)cbw->dCBWDataTransferLength,
                                 DEVICE_ADDRESS_MASS,
                                 &usb_state->storage_device[0].endpoints[ENDPOINT_BULK_OUT]),
            x_printf("Err8 %d ", result));
    }
  }

  _scsi_command_status_wrapper csw;
  CHECK(hw_data_in_transfer((uint8_t *)&csw,
                            sizeof(_scsi_command_status_wrapper),
                            DEVICE_ADDRESS_MASS,
                            &usb_state->storage_device[0].endpoints[ENDPOINT_BULK_IN]),
        x_printf("Err9 %d ", result));

  if (csw.cbwstatus != 0) {
    // printf(" cbwstatus(%d, %d, %d, %d, %d, %d, %d) ",
    //   csw.cbwtag[0],
    //   csw.cbwtag[3],
    //   csw.cbwresidue[0],
    //   csw.cbwresidue[1],
    //   csw.cbwresidue[2],
    //   csw.cbwresidue[3],
    //   csw.cbwstatus
    //   );
    return 99;
  }

  return USB_ERR_OK;
}

_scsi_read_capacity scsi_read_capacity = {0x25, 0, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0}};

usb_error get_scsi_read_capacity(scsi_read_capacity_result *cap_result) {
  usb_error         result;
  _usb_state *const usb_state = get_usb_work_area();

  cbw_scsi_read_capacity cbw_scsi;
  cbw_scsi.cbw           = scsi_command_block_wrapper;
  cbw_scsi.read_capacity = scsi_read_capacity;

  cbw_scsi.cbw.bCBWLUN                = 0;
  cbw_scsi.cbw.bCBWCBLength           = sizeof(_scsi_read_capacity);
  cbw_scsi.cbw.dCBWDataTransferLength = sizeof(scsi_read_capacity_result);
  cbw_scsi.cbw.dCBWTag[0]             = ++usb_state->storage_device[0].config.tag;

  CHECK(do_scsi_cmd(usb_state, &cbw_scsi.cbw, cap_result, false));

  return result;
}

_scsi_packet_inquiry scsi_packet_inquiry = {0x12, 0, 0, 0, 0x24, 0, {0, 0, 0, 0, 0, 0}};

usb_error get_scsi_inquiry(scsi_inquiry_result *inq_result) {
  usb_error         result;
  _usb_state *const usb_state = get_usb_work_area();

  cbw_scsi_inquiry cbw_scsi;
  cbw_scsi.cbw     = scsi_command_block_wrapper;
  cbw_scsi.inquiry = scsi_packet_inquiry;

  cbw_scsi.cbw.bCBWLUN                = 0;
  cbw_scsi.cbw.bCBWCBLength           = sizeof(_scsi_packet_inquiry);
  cbw_scsi.cbw.dCBWDataTransferLength = 0x24;
  cbw_scsi.cbw.dCBWTag[0]             = ++usb_state->storage_device[0].config.tag;

  CHECK(do_scsi_cmd(usb_state, &cbw_scsi.cbw, inq_result, false), x_printf("ErrI %d", result));

  return USB_ERR_OK;
}

setup_packet cmd_get_max_luns = {0b10100001, 0b11111110, {0, 0}, {0, 0}, 1};

usb_error get_scsi_max_luns() {
  usb_error         result;
  _usb_state *const usb_state = get_usb_work_area();

  setup_packet cmd;
  cmd           = cmd_get_max_luns;
  cmd.bIndex[0] = 0; // TODO get retrieve interface_id;

  uint8_t       buffer[4];
  const uint8_t max_packet_size = usb_state->storage_device[0].config.max_packet_size;

  result = hw_control_transfer(&cmd, buffer, DEVICE_ADDRESS_MASS, max_packet_size);

  x_printf("max-luns %d, %d, %d\r\n", result, max_packet_size, buffer[0]);

  return result;
}

usb_error scsi_test() {
  _usb_state *const usb_state = get_usb_work_area();

  cbw_scsi_test cbw_scsi;
  cbw_scsi.cbw = scsi_command_block_wrapper;
  memset(&cbw_scsi.test, 0, sizeof(_scsi_packet_test));

  cbw_scsi.cbw.bCBWLUN                = 0;
  cbw_scsi.cbw.bCBWCBLength           = sizeof(_scsi_packet_test);
  cbw_scsi.cbw.dCBWDataTransferLength = 0;
  cbw_scsi.cbw.dCBWTag[0]             = ++usb_state->storage_device[0].config.tag;

  return do_scsi_cmd(usb_state, &cbw_scsi.cbw, 0, false);
}

_scsi_packet_request_sense scsi_packet_request_sense = {0x03, 0, 0, 0, 18, 0, {0, 0, 0, 0, 0, 0}};

usb_error scsi_request_sense() __z88dk_fastcall {
  _usb_state *const usb_state = get_usb_work_area();

  uint8_t buffer[18];

  cbw_scsi_request_sense cbw_scsi;
  cbw_scsi.cbw           = scsi_command_block_wrapper;
  cbw_scsi.request_sense = scsi_packet_request_sense;

  cbw_scsi.cbw.bCBWLUN                = 0;
  cbw_scsi.cbw.bCBWCBLength           = sizeof(_scsi_packet_request_sense);
  cbw_scsi.cbw.dCBWDataTransferLength = 18;
  cbw_scsi.cbw.dCBWTag[0]             = ++usb_state->storage_device[0].config.tag;

  return do_scsi_cmd(usb_state, &cbw_scsi.cbw, buffer, false);
}

usb_error wait_for_mounting() {
  while (scsi_test()) {
    scsi_request_sense();
  }

  return USB_ERR_OK;
}