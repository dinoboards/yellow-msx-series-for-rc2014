#include "ufi.h"
#include "hw.h"
#include "usb.h"
#include <delay.h>
#include <stdbool.h>
#include <stdlib.h>

#include "print.h"
#include <string.h>
// ; -----------------------------------------------------------------------------
// ; USB_DATA_IN_TRANSFER: Perform a USB data IN transfer
// ;
// ; This routine differs from HW_DATA_IN_TRANSFER in that:
// ;
// ; - Passing the device address is not needed
// ; - Passing endpoint max packet size is not needed, it's taken from work area
// ; - Endpoint number is not (directly) passed
// ; - It manages the state of the toggle bit in work area
// ; -----------------------------------------------------------------------------
// ; Input:  buffer => HL = Address of a buffer for the received data
// ;         buffer_size => BC = Data length
// ;         send => Cy = 0 for bulk IN endpoint, 1 for interrupt endpoint
// ; Output: A  = USB error code
// ;         BC = Amount of data actually received
usb_error usb_data_in_transfer(_usb_state *const       usb_state,
                               uint8_t *const          buffer,
                               const uint16_t          buffer_size,
                               const usb_endpoint_type endpoint_type) {

  usb_error result;

  if (buffer_size == 0)
    return USB_ERR_OK;

  endpoint_param *const endpoint = &usb_state->endpoints[endpoint_type];

  result = hw_data_in_transfer(buffer, buffer_size, DEVICE_ADDRESS_FLOPPY, endpoint);

  if (result == USB_ERR_STALL) {
    usb_clear_endpoint_halt(usb_state, endpoint_type);
    return USB_ERR_STALL;
  }

  return usb_process_error(usb_state, result);
}

// ; -----------------------------------------------------------------------------
// ; USB_DATA_OUT_TRANSFER: Perform a USB data OUT transfer
// ;
// ; This routine differs from HW_DATA_OUT_TRANSFER in that:
// ;
// ; - Passing the device address is not needed
// ; - Passing endpoint max packet size is not needed, it's taken from work area
// ; - Endpoint number is assumed to be the bulk out one
// ; - It manages the state of the toggle bit in work area
// ; -----------------------------------------------------------------------------
// ; Input:  HL = Address of a buffer for the data to send data
// ;         BC = Data length
// ; Output: A  = USB error code
inline usb_error usb_data_out_transfer(_usb_state *const usb_state, uint8_t *const buffer, const uint16_t buffer_size) {

  usb_error result;

  endpoint_param *const endpoint = &usb_state->endpoints[ENDPOINT_BULK_OUT];

  result = hw_data_out_transfer(buffer, buffer_size, DEVICE_ADDRESS_FLOPPY, endpoint);

  if (result == USB_ERR_STALL) {
    usb_clear_endpoint_halt(usb_state, ENDPOINT_BULK_OUT);
    return USB_ERR_STALL;
  }

  return result;
}

// IX => adsc ;In: IX=Prepared ADSC,
// HL => command,
// DE => buffer,
// BC => buffer_size?? data length,
// Cy => send (0 to receive, 1 to write)
// Returns:
//  A = USB error code (STALL if non-zero ASC),
//  BC=data transferred
//  D=ASC if STALL (0 if not available)
usb_error usb_execute_cbi_core_no_clear(_usb_state *const         usb_state,
                                        const setup_packet *const adsc,
                                        const uint8_t *const      cmd,
                                        const bool                send,
                                        const uint16_t            buffer_size,
                                        uint8_t *const            buffer,
                                        uint8_t *const            asc) {

  usb_error result;

  result = usb_control_transfer(usb_state, adsc, cmd);

  if (result != USB_ERR_OK) {
    *asc = 0;

    return result;
  }

  //_USB_EXE_CBI_STEP_2
  if (send) {
    result = usb_data_out_transfer(usb_state, buffer, buffer_size);
  } else {
    result = usb_data_in_transfer(usb_state, buffer, buffer_size, ENDPOINT_BULK_IN);
  }

  if (result != USB_ERR_OK) {
    *asc = 0;

    // Token error here, asc = 0

    x_printf(" a2(%d) ", result);
    return result;
  }

  uint8_t sense_codes[2] = {255, 255};

  result = usb_data_in_transfer(usb_state, sense_codes, 2, ENDPOINT_INTERRUPT_IN);

  if (result != USB_ERR_OK)
    return result;

  if (sense_codes[0] == 255 && sense_codes[1] == 255)
    return USB_ERR_STALL;

  if (sense_codes[0] == 0 && sense_codes[1] == 0)
    return USB_ERR_OK;

  *asc = sense_codes[0];

  return USB_ERR_STALL;
}

// ; -----------------------------------------------------------------------------
// ; USB_PROCESS_ERROR
// ;
// ; If USB error is "device disconnected", clear work area.
// ; Otherwise, if it's not STALL, reset device.
// ; -----------------------------------------------------------------------------
usb_error usb_process_error(_usb_state *const usb_state, const usb_error result) {
  (void)usb_state;

  if (result == USB_ERR_OK)
    return result;

  if (result == USB_ERR_NO_DEVICE)
    return result;

  if (result == USB_ERR_TIMEOUT)
    return result;

  if (result == USB_ERR_CH376_BLOCKED)
    return USB_ERR_TIMEOUT;

  // if (result != USB_ERR_STALL) {
  // yprintf(70, " Fail %02x ", result);
  // hw_bus_reset();
  // usb_init_dev();

  // usb_state->error = result;
  // usb_state->asc => ??;
  // usb_state->ascq => ??;
  // }

  return result;
}

// ; -----------------------------------------------------------------------------
// ; USB_CONTROL_TRANSFER: Perform a USB control transfer on endpoint 0
// ;
// ; The size and direction of the transfer are taken from the contents
// ; of the setup packet.
// ;
// ; This routine differs from HW_CONTROL_TRANSFER in that:
// ;
// ; - Passing the device address is not needed
// ; - Passing endpoint 0 max packet size is not needed, it's taken from work area
// ; -----------------------------------------------------------------------------
// ; Input:  cmd => HL = Address of a 8 byte buffer with the setup packet
// ;         buffer => DE = Address of the input or output data buffer
// ; Output: A  = USB error code
// ;         BC = Amount of data actually transferred (if IN transfer and no error)
usb_error usb_control_transfer(_usb_state *const usb_state, const setup_packet *const cmd, uint8_t *const buffer) {
  usb_error result;

  const uint8_t max_packet_size = usb_state->floppy_config.max_packet_size;

  result = hw_control_transfer(cmd, buffer, DEVICE_ADDRESS_FLOPPY, max_packet_size);

  if (result == USB_ERR_OK)
    return result;

  return usb_process_error(usb_state, result);
}

setup_packet usb_cmd_clear_endpoint_halt = {2, 1, {0, 0}, {255, 0}, 0}; //    ;byte 4 is the endpoint to be cleared

// ; -----------------------------------------------------------------------------
// ; USB_CLEAR_ENDPOINT_HALT
// ;
// ; Also clears the toggle bit in the work area.
// ; -----------------------------------------------------------------------------
// ; Input: A = which endpoint to clear:
// ;            0: bulk OUT
// ;            1: bulk IN
// ;            2: interrupt IN
usb_error usb_clear_endpoint_halt(_usb_state *const usb_state, const usb_endpoint_type endpoint_type) {
  setup_packet cmd;
  cmd           = usb_cmd_clear_endpoint_halt;
  cmd.bIndex[0] = usb_state->endpoints[endpoint_type].number;

  const uint8_t result = usb_control_transfer(usb_state, &cmd, (uint8_t *)0);

  usb_state->endpoints[endpoint_type].toggle = false;

  return result;
}

// IX => adsc ;In: IX=Prepared ADSC,
// HL => command,
// DE => buffer,
// BC => buffer_size?? data length,
// Cy => send (0 to receive, 1 to write)
// Returns:
//  A = USB error code (STALL if non-zero ASC),
//  BC=data transferred
//  D=ASC if STALL (0 if not available)
usb_error usb_execute_cbi_core(_usb_state *const         usb_state,
                               const setup_packet *const adsc,
                               const uint8_t *const      cmd,
                               const bool                send,
                               const uint16_t            buffer_size,
                               uint8_t *const            buffer) {
  uint8_t asc;

  const usb_error result = usb_execute_cbi_core_no_clear(usb_state, adsc, cmd, send, buffer_size, buffer, &asc);

  if (send)
    return result;

  if (result != USB_ERR_STALL)
    return result;

  if (asc != 0)
    return USB_ERR_STALL;

  usb_clear_endpoint_halt(usb_state, ENDPOINT_BULK_IN);
  return USB_ERR_STALL;
}

setup_packet        cbi_adsc              = {0x21, 0, {0, 0}, {255, 0}, 12}; // ;4th byte is interface number
ufi_request_inquiry ufi_cmd_request_sense = {0x03, 0, 0, 0, 18, 0, {0, 0, 0, 0, 0, 0}};

// ; Input:  HL => cmd Address of the 12 byte command to execute
// ;         DE => buffer Address of the input or output data buffer
// ;         BC => buffer_size Length of data to send or receive
// ;         Cy => send, 0 to receive data, 1 to send data
// ; Output: A  = USB error code
// ;         BC = amount_received of data actually transferred (if IN transfer)
// ;         D  = ASC (if no error)
// ;         E  = ASCQ (if no error)
usb_error usb_execute_cbi(_usb_state *const           usb_state,
                          const uint8_t *const        cmd,
                          const bool                  send,
                          const uint16_t              buffer_size,
                          uint8_t *const              buffer,
                          ufi_response_inquiry *const response_inquiry) {
  uint8_t       result;
  const uint8_t interface_number = usb_state->interface_number;

  setup_packet setup_cmd_packet;
  setup_cmd_packet           = cbi_adsc;
  setup_cmd_packet.bIndex[0] = interface_number;

  result = usb_execute_cbi_core(usb_state, &setup_cmd_packet, cmd, send, buffer_size, buffer);

  if (result != USB_ERR_STALL)
    return result;

  return usb_execute_cbi_core(usb_state,
                              &setup_cmd_packet,
                              (uint8_t *)&ufi_cmd_request_sense,
                              false,
                              sizeof(ufi_response_inquiry),
                              (uint8_t *)response_inquiry);
}

// ; Input:  HL => cmd Address of the 12 byte command to execute
// ;         DE => buffer Address of the input or output data buffer
// ;         BC => buffer_size Length of data to send or receive
// ;         Cy => send, 0 to receive data, 1 to send data
//           A => retry_on_media_change
// ; Output: A  = USB error code
// ;         BC = amount_received of data actually transferred (if IN transfer)
// ;         D  = ASC (if no error)
// ;         E  = ASCQ (if no error)
usb_error usb_execute_cbi_with_retry(_usb_state *const    usb_state,
                                     const uint8_t *const cmd,
                                     const bool           send,
                                     const bool           retry_on_media_change,
                                     const uint16_t       buffer_size,
                                     uint8_t *const       buffer) {

  uint8_t              result;
  ufi_response_inquiry response_inquiry;

  response_inquiry.asc  = 0;
  response_inquiry.ascq = 0;

retry:
  result = usb_execute_cbi(usb_state, (uint8_t *)cmd, send, buffer_size, buffer, &response_inquiry);

  const uint8_t asc       = response_inquiry.asc;
  const uint8_t ascq      = response_inquiry.ascq;
  const uint8_t sense_key = response_inquiry.sense_key;

  if (result)
    return result;

  if (asc == 0)
    return 0;

  if (asc == 0x17 || asc == 0x18)
    return asc;

  if (asc == 4 && (ascq == 1 || ascq == 0xFF)) // Retry if ASC=4 and *ASCQ=1 (unit becoming ready) or FFh (unit busy)
    goto retry;

  if (asc == 0x29) // device powered
    goto retry;

  if (asc == 0x28 && retry_on_media_change) // Retry "media changed" only if we were instructed to do so
    goto retry;

  if (asc == 0x28)
    return USB_ERR_MEDIA_CHANGED;

  if (asc == 0x3A)
    return USB_ERR_MEDIA_NOT_PRESENT;

  return result;
}

ufi_request_inquiry packet_inquiry = {0x12, 0, 0, 0, 0x24, 0, {0, 0, 0, 0, 0, 0}};

usb_error ufi_inquiry(_usb_state *const usb_state, ufi_inquiry_response const *response) {

  return usb_execute_cbi_with_retry(
      usb_state, (uint8_t *)&packet_inquiry, false, true, sizeof(ufi_inquiry_response), (uint8_t *)response);
}

ufi_read_format_capacities packet_read_format_capacities = {0x23, 0, {0, 0, 0, 0, 0}, {0, 12}, {0, 0, 0}};

usb_error ufi_capacity(_usb_state *const usb_state, ufi_format_capacities_response const *response) {
  test_disk(usb_state); //, x_printf(" XX %d ", result));

  return usb_execute_cbi_with_retry(usb_state,
                                    (uint8_t *)&packet_read_format_capacities,
                                    false,
                                    true,
                                    sizeof(ufi_format_capacities_response),
                                    (uint8_t *)response);
}

usb_error run_test_unit_ready(_usb_state *const usb_state) {
  ufi_request_test_unit_ready cmd;
  memset(&cmd, 0, sizeof(ufi_request_test_unit_ready));

  return usb_execute_cbi_with_retry(usb_state, (uint8_t *)&cmd, false, false, 0, (uint8_t *)0);
}

uint8_t test_disk(_usb_state *const usb_state) {
  usb_error result;
  result = run_test_unit_ready(usb_state);

  if (result == USB_ERR_MEDIA_CHANGED || result == USB_TOKEN_OUT_OF_SYNC) {
    result = run_test_unit_ready(usb_state);
    return (result == USB_ERR_OK) ? USB_ERR_MEDIA_CHANGED : result;
  }

  return result;
}

uint8_t packet_ufi_write_sector[] = {0x2A, 0, 0, 0, 255, 255, 0, 0, 1, 0, 0, 0};

usb_error ufi_write_sector(_usb_state *const usb_state, const uint16_t sector_number, const uint8_t *const buffer) {
  uint8_t cmd[12];
  memcpy(cmd, packet_ufi_write_sector, 12);

  cmd[4] = sector_number >> 8;
  cmd[5] = sector_number & 0xFF;

  return usb_execute_cbi_with_retry(usb_state, (uint8_t *)&cmd, true, true, 512, (uint8_t *)buffer);
}

uint8_t packet_ufi_read_sector[] = {0x28, 0, 0, 0, 255, 255, 0, 0, 1, 0, 0, 0};

usb_error ufi_read_sector(_usb_state *const usb_state, const uint16_t sector_number, uint8_t *const buffer) {
  uint8_t cmd[12];
  memcpy(cmd, packet_ufi_read_sector, 12);

  cmd[4] = sector_number >> 8;
  cmd[5] = sector_number & 0xFF;

  return usb_execute_cbi_with_retry(usb_state, (uint8_t *)&cmd, false, true, 512, buffer);
}
