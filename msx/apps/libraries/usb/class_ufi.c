#include "class_ufi.h"
#include "hw.h"
#include <delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

usb_error usb_data_in_transfer(device_config *const    storage_device,
                               uint8_t *const          buffer,
                               const uint16_t          buffer_size,
                               const usb_endpoint_type endpoint_type) {

  usb_error result;

  if (buffer_size == 0)
    return USB_ERR_OK;

  endpoint_param *const endpoint = &storage_device->endpoints[endpoint_type];

  result = hw_data_in_transfer(buffer, buffer_size, storage_device->address, endpoint);

  if (result == USB_ERR_STALL) {
    usb_clear_endpoint_halt(storage_device, endpoint_type);
    return USB_ERR_STALL;
  }

  return usb_process_error(result);
}

inline usb_error usb_data_out_transfer(device_config *const storage_device, uint8_t *const buffer, const uint16_t buffer_size) {

  usb_error result;

  endpoint_param *const endpoint = &storage_device->endpoints[ENDPOINT_BULK_OUT];

  result = hw_data_out_transfer(buffer, buffer_size, storage_device->address, endpoint);

  if (result == USB_ERR_STALL) {
    usb_clear_endpoint_halt(storage_device, ENDPOINT_BULK_OUT);
    return USB_ERR_STALL;
  }

  return result;
}

usb_error usb_execute_cbi_core_no_clear(device_config *const      storage_device,
                                        const setup_packet *const adsc,
                                        const uint8_t *const      cmd,
                                        const bool                send,
                                        const uint16_t            buffer_size,
                                        uint8_t *const            buffer,
                                        uint8_t *const            asc) {

  usb_error result;

  result = usb_control_transfer(storage_device, adsc, cmd);

  if (result != USB_ERR_OK) {
    *asc = 0;

    return result;
  }

  if (send) {
    result = usb_data_out_transfer(storage_device, buffer, buffer_size);
  } else {
    result = usb_data_in_transfer(storage_device, buffer, buffer_size, ENDPOINT_BULK_IN);
  }

  if (result != USB_ERR_OK) {
    *asc = 0;
    // Token error here, asc = 0
    CHECK(result);

    return result;
  }

  uint8_t sense_codes[2] = {255, 255};

  result = usb_data_in_transfer(storage_device, sense_codes, 2, ENDPOINT_INTERRUPT_IN);

  if (result != USB_ERR_OK)
    return result;

  if (sense_codes[0] == 255 && sense_codes[1] == 255)
    return USB_ERR_STALL;

  if (sense_codes[0] == 0 && sense_codes[1] == 0)
    return USB_ERR_OK;

  *asc = sense_codes[0];

  return USB_ERR_STALL;
}

usb_error usb_process_error(const usb_error result) {
  if (result == USB_ERR_OK)
    return result;

  if (result == USB_ERR_NO_DEVICE)
    return result;

  if (result == USB_ERR_TIMEOUT)
    return result;

  if (result == USB_ERR_CH376_BLOCKED)
    return USB_ERR_TIMEOUT;

  return result;
}

usb_error usb_control_transfer(device_config *const storage_device, const setup_packet *const cmd, uint8_t *const buffer) {
  usb_error result;

  const uint8_t max_packet_size = storage_device->max_packet_size;

  result = hw_control_transfer(cmd, buffer, storage_device->address, max_packet_size);

  if (result == USB_ERR_OK)
    return result;

  return usb_process_error(result);
}

setup_packet usb_cmd_clear_endpoint_halt = {2, 1, {0, 0}, {255, 0}, 0}; //    ;byte 4 is the endpoint to be cleared

usb_error usb_clear_endpoint_halt(device_config *const storage_device, const usb_endpoint_type endpoint_type) {
  setup_packet cmd;
  cmd           = usb_cmd_clear_endpoint_halt;
  cmd.bIndex[0] = storage_device->endpoints[endpoint_type].number;

  const uint8_t result = usb_control_transfer(storage_device, &cmd, (uint8_t *)0);

  storage_device->endpoints[endpoint_type].toggle = false;

  return result;
}

usb_error usb_execute_cbi_core(device_config *const      storage_device,
                               const setup_packet *const adsc,
                               const uint8_t *const      cmd,
                               const bool                send,
                               const uint16_t            buffer_size,
                               uint8_t *const            buffer) {
  uint8_t asc;

  const usb_error result = usb_execute_cbi_core_no_clear(storage_device, adsc, cmd, send, buffer_size, buffer, &asc);

  if (send)
    return result;

  if (result != USB_ERR_STALL)
    return result;

  if (asc != 0)
    return USB_ERR_STALL;

  usb_clear_endpoint_halt(storage_device, ENDPOINT_BULK_IN);
  return USB_ERR_STALL;
}

setup_packet        cbi_adsc              = {0x21, 0, {0, 0}, {255, 0}, 12}; // ;4th byte is interface number
ufi_request_inquiry ufi_cmd_request_sense = {0x03, 0, 0, 0, 18, 0, {0, 0, 0, 0, 0, 0}};

usb_error usb_execute_cbi(device_config *const        storage_device,
                          const uint8_t *const        cmd,
                          const bool                  send,
                          const uint16_t              buffer_size,
                          uint8_t *const              buffer,
                          ufi_response_inquiry *const response_inquiry) {
  uint8_t       result;
  const uint8_t interface_number = storage_device->interface_number;

  setup_packet setup_cmd_packet;
  setup_cmd_packet           = cbi_adsc;
  setup_cmd_packet.bIndex[0] = interface_number;

  result = usb_execute_cbi_core(storage_device, &setup_cmd_packet, cmd, send, buffer_size, buffer);

  if (result != USB_ERR_STALL)
    return result;

  return usb_execute_cbi_core(storage_device, &setup_cmd_packet, (uint8_t *)&ufi_cmd_request_sense, false,
                              sizeof(ufi_response_inquiry), (uint8_t *)response_inquiry);
}

usb_error usb_execute_cbi_with_retry(device_config *const storage_device,
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
  result = usb_execute_cbi(storage_device, (uint8_t *)cmd, send, buffer_size, buffer, &response_inquiry);

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

usb_error ufi_inquiry(device_config *const storage_device, ufi_inquiry_response const *response) {

  return usb_execute_cbi_with_retry(storage_device, (uint8_t *)&packet_inquiry, false, true, sizeof(ufi_inquiry_response),
                                    (uint8_t *)response);
}

ufi_read_format_capacities packet_read_format_capacities = {0x23, 0, {0, 0, 0, 0, 0}, {0, 12}, {0, 0, 0}};

usb_error ufi_capacity(device_config *const storage_device, ufi_format_capacities_response const *response) {
  ufi_test_disk(storage_device);

  return usb_execute_cbi_with_retry(storage_device, (uint8_t *)&packet_read_format_capacities, false, true,
                                    sizeof(ufi_format_capacities_response), (uint8_t *)response);
}

usb_error run_test_unit_ready(device_config *const storage_device) {
  ufi_request_test_unit_ready cmd;
  memset(&cmd, 0, sizeof(ufi_request_test_unit_ready));

  return usb_execute_cbi_with_retry(storage_device, (uint8_t *)&cmd, false, false, 0, (uint8_t *)0);
}

usb_error ufi_test_disk(device_config *const storage_device) {
  usb_error result;
  result = run_test_unit_ready(storage_device);

  if (result == USB_ERR_MEDIA_CHANGED || result == USB_TOKEN_OUT_OF_SYNC) {
    result = run_test_unit_ready(storage_device);
    return (result == USB_ERR_OK) ? USB_ERR_MEDIA_CHANGED : result;
  }

  return result;
}

usb_error ufi_read_write_sector(device_config *const storage_device,
                                const bool           send,
                                const uint16_t       sector_number,
                                const uint8_t        sector_count,
                                const uint8_t *const buffer) {
  ufi_read_write cmd;
  memset(&cmd, 0, sizeof(ufi_read_write));
  cmd.operation_code     = send ? 0x2A : 0x28;
  cmd.lba[2]             = sector_number >> 8;
  cmd.lba[3]             = sector_number & 0xFF;
  cmd.transfer_length[1] = sector_count;

  return usb_execute_cbi_with_retry(storage_device, (uint8_t *)&cmd, send, true, 512 * sector_count, (uint8_t *)buffer);
}
