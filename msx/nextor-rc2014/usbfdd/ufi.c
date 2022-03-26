#include "ufi.h"
#include "hw.h"
#include "usb.h"
#include <stdbool.h>
#include <stdlib.h>

// IX => adsc ;In: IX=Prepared ADSC,
// HL => command,
// DE => buffer,
// BC => buffer_size?? data length,
// Cy => send (0 to receive, 1 to write)
// Returns:
//  A = USB error code (STALL if non-zero ASC),
//  BC=data transferred
//  D=ASC if STALL (0 if not available)
uint8_t usb_execute_cbi_core_no_clear(_usb_state *const    usb_state,
                                      const uint8_t *const adsc,
                                      const uint8_t *const cmd,
                                      const bool           send,
                                      const uint16_t       buffer_size,
                                      const uint8_t *const buffer,
                                      const uint8_t *      asc,
                                      const uint16_t *     amount_transferred) {

  (void)usb_state;
  (void)adsc;
  (void)cmd;
  (void)send;
  (void)buffer_size;
  (void)buffer;
  (void)asc;
  (void)amount_transferred;

  return 99;

  // uint8_t result;

  // result = usb_control_transfer();

  // if (!result) {
  //   *asc                = 0;
  //   *amount_transferred = 0;
  // }

  // //_USB_EXE_CBI_STEP_2
  // if (send) {

  //   return 99;
  // }

  // // _USB_EXE_CBI_DATA_IN
  // result = usb_data_in_transfer(endpoint : 0);
  // if (!result) {
  //   *asc                = 0;
  //   *amount_transferred = 0;
  // }

  // // _USB_EXE_CBI_STEP_3
  // result = usb_data_in_transfer(amount_transferred);
  // if (!result) {
  //   *asc = 0 ;
  // }

  // if (*amount_transferred == 0) {
  //   *asc = 0 ;
  //   return USB_ERR_STALL;
  // }

  // // ????
}

uint8_t usb_process_error(const uint8_t result) {
  (void)result;
  return 99;
}

// HL => cmd
// DE => buffer
// BC => *amount_transferred
uint8_t usb_control_transfer(const _usb_state *const   usb_state,
                             const setup_packet *const cmd,
                             uint8_t *const            buffer,
                             uint16_t *                amount_transferred) {
  uint8_t result;

  const uint8_t max_packet_size = usb_state->endpoints[ENDPOINT_INTERRUPT_IN].max_packet_size;

  result = hw_control_transfer(cmd, buffer, DEVICE_ADDRESS, max_packet_size, amount_transferred);

  if (result == 0)
    return result;

  return usb_process_error(result);
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
void usb_clear_endpoint_halt(const _usb_state *const usb_state, const uint8_t endpoint_type) {
  setup_packet cmd;
  cmd           = usb_cmd_clear_endpoint_halt;
  cmd.bIndex[0] = usb_state->endpoints[endpoint_type].number;

  // usb_control_transfer()
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
uint8_t usb_execute_cbi_core(_usb_state *const    usb_state,
                             const uint8_t *const adsc,
                             const uint8_t *const cmd,
                             const bool           send,
                             const uint16_t       buffer_size,
                             const uint8_t *const buffer,
                             const uint8_t *      asc,
                             const uint16_t *     amount_transferred) {
  uint8_t result;
  if (send) {
    return usb_execute_cbi_core_no_clear(usb_state, adsc, cmd, send, buffer_size, buffer, asc, amount_transferred);
  }

  result = usb_execute_cbi_core_no_clear(usb_state, adsc, cmd, send, buffer_size, buffer, asc, amount_transferred);
  if (result != USB_ERR_STALL)
    return result;

  if (asc != 0)
    return USB_ERR_STALL;

  usb_clear_endpoint_halt(usb_state, 1);
  return USB_ERR_STALL;
}

setup_packet        cbi_adsc              = {0x21, 0, {0, 0}, {255, 0}, 12}; // ;4th byte is interface number
ufi_request_inquiry ufi_cmd_request_sense = {3, 0, 0, 0, 18, 0, {0, 0, 0, 0, 0, 0}};

// ; Input:  HL => cmd Address of the 12 byte command to execute
// ;         DE => buffer Address of the input or output data buffer
// ;         BC => buffer_size Length of data to send or receive
// ;         Cy => send, 0 to receive data, 1 to send data
// ; Output: A  = USB error code
// ;         BC = amount_received of data actually transferred (if IN transfer)
// ;         D  = ASC (if no error)
// ;         E  = ASCQ (if no error)
uint8_t usb_execute_cbi(_usb_state *const    usb_state,
                        const uint8_t *const cmd,
                        const bool           send,
                        const uint16_t       buffer_size,
                        uint8_t *const       buffer,
                        uint8_t *const       asc,
                        uint8_t *const       ascq,
                        uint16_t *const      amount_transferred) {
  uint8_t result;

  *asc  = 0;
  *ascq = 0;

  const uint8_t interface_number = usb_state->interface_number;

  setup_packet setup_cmd_packet;
  setup_cmd_packet.bIndex[0] = interface_number;
  ufi_response_inquiry response_inquiry;

  result =
      usb_execute_cbi_core(usb_state, (uint8_t *)&setup_cmd_packet, cmd, send, buffer_size, buffer, asc, amount_transferred);

  if (result != USB_ERR_STALL)
    return result;

  result = usb_execute_cbi_core(usb_state,
                                (uint8_t *)&cbi_adsc,
                                (uint8_t *)&ufi_cmd_request_sense,
                                false,
                                sizeof(ufi_response_inquiry),
                                (uint8_t *)&response_inquiry,
                                asc,
                                amount_transferred);

  *asc  = response_inquiry.asc;
  *ascq = response_inquiry.ascq;

  if (result != USB_ERR_STALL)
    return result;

  return 99;
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
uint8_t usb_execute_cbi_with_retry(_usb_state *const                usb_state,
                                   const ufi_request_inquiry *const cmd,
                                   const bool                       send,
                                   const uint16_t                   buffer_size,
                                   const uint8_t                    retry_on_media_change,
                                   uint8_t *const                   buffer,
                                   uint16_t *const                  amount_transferred) {

  uint8_t result;
  uint8_t asc;
  uint8_t ascq;

retry:
  result = usb_execute_cbi(usb_state, (uint8_t *)cmd, send, buffer_size, buffer, &asc, &ascq, amount_transferred);

  if (result)
    return result;

  if (asc == 0)
    return 0;

  if (asc == 0x17 || asc == 0x18)
    return asc;

  if (asc == 4 && (ascq == 1 || ascq == 0xFF)) // Retry if ASC=4 and ASCQ=1 (unit becoming ready) or FFh (unit busy)
    goto retry;

  if (asc == 0x29) // device powered
    goto retry;

  if (asc == 0x28 && retry_on_media_change) // Retry "media changed" only if we were instructed to do so
    goto retry;

  return result;
}

ufi_request_inquiry packet_inquiry = {0x12, 0, 0, 0, 0x24, 0, {0, 0, 0, 0, 0, 0}};

uint8_t ufi_inquiry(_usb_state *const usb_state) {
  uint8_t  buffer[38];
  uint16_t amount_transferred;

  return usb_execute_cbi_with_retry(usb_state, &packet_inquiry, false, 38, true, buffer, &amount_transferred);
}