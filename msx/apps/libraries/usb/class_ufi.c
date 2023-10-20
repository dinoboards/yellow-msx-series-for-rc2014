#include "class_ufi.h"
#include "protocol.h"
#include <delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "enumerate_trace.h"

const ufi_request_sense_command          _ufi_cmd_request_sense          = {0x03, 0, 0, 0, 0, 18, {0, 0, 0, 0, 0, 0, 0}};
const ufi_read_format_capacities_command _ufi_cmd_read_format_capacities = {0x23, 0, 0, {0, 0, 0, 0, 0}, {0, 12}, {0, 0, 0}};
const ufi_inquiry_command                _ufi_cmd_inquiry                = {0x12, 0, 0, 0, 0, 0, 0x24, {0, 0, 0, 0, 0, 0, 0}};
const ufi_format_command                 _ufi_cmd_format                 = {0x04, 7, 0, 1, 0, 0, {0, 0}, {0, 0}, {0, 0}, {0, 0, 0}};
const ufi_send_diagnostic_command        _ufi_cmd_send_diagnostic        = {0x1D, 0, 0, 1, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

uint8_t wait_for_device_ready(device_config *const storage_device, const uint16_t timeout_period) {
  usb_error                  result;
  ufi_request_sense_response sense;

  int16_t timeout = get_future_time(from_ms(timeout_period));

  do {
    memset(&sense, 0, sizeof(sense));
    result = ufi_test_unit_ready(storage_device, &sense);

    // trace_printf("wait_for_device_ready: %d, %02X %02X %02X\r\n", result, sense.sense_key, sense.asc, sense.ascq);

    if ((result == USB_ERR_OK && sense.sense_key == 0) || is_time_past(timeout))
      break;

    delay_medium();

  } while (true);

  return result | sense.sense_key;
}

usb_error ufi_test_unit_ready(device_config *const storage_device, ufi_request_sense_response const *response) {
  usb_error                   result;
  ufi_test_unit_ready_command ufi_cmd_request_test_unit_ready;
  memset(&ufi_cmd_request_test_unit_ready, 0, sizeof(ufi_test_unit_ready_command));

  usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_request_test_unit_ready, false, 0, NULL, NULL);

  ufi_request_sense_command ufi_cmd_request_sense;
  ufi_cmd_request_sense = _ufi_cmd_request_sense;

  result = usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_request_sense, false, sizeof(ufi_request_sense_response),
                           (uint8_t *)response, NULL);

  RETURN_CHECK(result);
}

usb_error ufi_request_sense(device_config *const storage_device, ufi_request_sense_response const *response) {
  ufi_request_sense_command ufi_cmd_request_sense;
  ufi_cmd_request_sense = _ufi_cmd_request_sense;

  usb_error result = usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_request_sense, false, sizeof(ufi_request_sense_response),
                                     (uint8_t *)response, NULL);

  RETURN_CHECK(result);
}

usb_error ufi_read_format_capacities(device_config *const storage_device, ufi_format_capacities_response const *response) {
  usb_error                          result;
  ufi_read_format_capacities_command ufi_cmd_read_format_capacities;

  ufi_cmd_read_format_capacities = _ufi_cmd_read_format_capacities;
  result = usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_read_format_capacities, false, 12, (uint8_t *)response, NULL);

  TRACE_USB_ERROR(result);
  CHECK(result);

  const uint8_t available_length = response->capacity_list_length;

  const uint8_t max_length =
      available_length > sizeof(ufi_format_capacities_response) ? sizeof(ufi_format_capacities_response) : available_length;

  ufi_read_format_capacities_command cmd;
  memcpy(&cmd, &ufi_cmd_read_format_capacities, sizeof(cmd));
  cmd.allocation_length[1] = max_length;

  result = usb_execute_cbi(storage_device, (uint8_t *)&cmd, false, max_length, (uint8_t *)response, NULL);

  TRACE_USB_ERROR(result);
  RETURN_CHECK(result);
}

usb_error ufi_inquiry(device_config *const storage_device, ufi_inquiry_response const *response) {
  ufi_inquiry_command ufi_cmd_inquiry;
  ufi_cmd_inquiry = _ufi_cmd_inquiry;

  usb_error result =
      usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_inquiry, false, sizeof(ufi_inquiry_response), (uint8_t *)response, NULL);

  RETURN_CHECK(result);
}

usb_error ufi_read_write_sector(device_config *const storage_device,
                                const bool           send,
                                const uint16_t       sector_number,
                                const uint8_t        sector_count,
                                const uint8_t *const buffer,
                                uint8_t *const       sense_codes) {
  ufi_read_write_command cmd;
  memset(&cmd, 0, sizeof(cmd));
  cmd.operation_code     = send ? 0x2E : 0x28;
  cmd.lba[2]             = sector_number >> 8;
  cmd.lba[3]             = sector_number & 0xFF;
  cmd.transfer_length[1] = sector_count;

  usb_error result = usb_execute_cbi(storage_device, (uint8_t *)&cmd, send, 512 * sector_count, (uint8_t *)buffer, sense_codes);

  RETURN_CHECK(result);
}

/**
 * Medium | Medium Type Code | Capacity | Tracks | Heads | Sectors/Track | Total Blocks | Block Length |
 * DD     | 1Eh              | 720 KB   | 80     | 2     | 9             | 1440 05A0h   | 512 0200h    |
 * HD     | 93h              | 1.25 MB  | 77     | 2     | 8             | 1232 04D0h   | 1024 0400h   |
 * HD     | 94h              | 1.44 MB  | 80     | 2     | 18            | 2880 0B40h   | 512 0200h    |
 */

usb_error ufi_format(device_config *const                        storage_device,
                     const uint8_t                               side,
                     const uint8_t                               track_number,
                     const ufi_format_capacity_descriptor *const format) {
  ufi_interrupt_status sense_codes;

  ufi_format_parameter_list parameter_list;
  memset(&parameter_list, 0, sizeof(parameter_list));

  ufi_format_command cmd;
  cmd = _ufi_cmd_format;
  // memcpy(&cmd, &_ufi_cmd_format, sizeof(cmd));

  cmd.track_number             = track_number;
  cmd.interleave[1]            = 0;
  cmd.parameter_list_length[1] = sizeof(parameter_list);

  parameter_list.defect_list_header.side                   = side;
  parameter_list.defect_list_header.immediate              = 0;
  parameter_list.defect_list_header.reserved2              = 0;
  parameter_list.defect_list_header.single_track           = 1;
  parameter_list.defect_list_header.dcrt                   = 1;
  parameter_list.defect_list_header.extend                 = 0;
  parameter_list.defect_list_header.fov                    = 1;
  parameter_list.defect_list_header.defect_list_length_msb = 0;
  parameter_list.defect_list_header.defect_list_length_lsb = 8;
  memcpy(&parameter_list.format_descriptor, (void *)format, sizeof(ufi_format_capacity_descriptor));

  usb_error result = usb_execute_cbi(storage_device, (uint8_t *)&cmd, true, sizeof(parameter_list), (uint8_t *)&parameter_list,
                                     (void *)&sense_codes);

  // trace_printf("ufi_format: %d, %02X %02X (len: %d)\r\n", result, sense_codes.bASC, sense_codes.bASCQ, sizeof(parameter_list));

  RETURN_CHECK(result);
}

usb_error ufi_send_diagnostics(device_config *const storage_device) {
  usb_error                   result;
  ufi_send_diagnostic_command ufi_cmd_send_diagnostic;

  ufi_cmd_send_diagnostic = _ufi_cmd_send_diagnostic;

  result = usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_send_diagnostic, true, 0, NULL, NULL);

  RETURN_CHECK(result);
}
