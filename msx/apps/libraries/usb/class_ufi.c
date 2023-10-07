#include "class_ufi.h"
#include "protocol.h"
#include <delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "enumerate_trace.h"

const ufi_request_sense_command          ufi_cmd_request_sense          = {0x03, 0, 0, 0, 0, 18, {0, 0, 0, 0, 0, 0, 0}};
const ufi_read_format_capacities_command ufi_cmd_read_format_capacities = {0x23, 0, 0, {0, 0, 0, 0, 0}, {0, 12}, {0, 0, 0}};
const ufi_inquiry_command                ufi_cmd_inquiry                = {0x12, 0, 0, 0, 0, 0, 0x24, {0, 0, 0, 0, 0, 0, 0}};
const ufi_format_command ufi_cmd_format = {0x04, 0, 0, 0, 0, 0, {0, 0}, {0, 0}, {0, sizeof(ufi_format_parameter_list)}, {0, 0}};

uint8_t wait_for_device_ready(device_config *const storage_device, const uint16_t timeout_period) {
  usb_error                  result;
  ufi_request_sense_response sense;

  int16_t timeout = get_future_time(from_ms(timeout_period));

  do {
    memset(&sense, 0, sizeof(sense));
    result = ufi_test_unit_ready(storage_device, &sense);

    trace_printf("wait_for_device_ready: %d, %02X %02X %02X\r\n", result, sense.sense_key, sense.asc, sense.ascq);

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

  result = usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_request_sense, false, sizeof(ufi_request_sense_response),
                           (uint8_t *)response, NULL);

  RETURN_CHECK(result);
}

usb_error ufi_request_sense(device_config *const storage_device, ufi_request_sense_response const *response) {
  usb_error result = usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_request_sense, false, sizeof(ufi_request_sense_response),
                                     (uint8_t *)response, NULL);

  RETURN_CHECK(result);
}

usb_error ufi_read_format_capacities(device_config *const storage_device, ufi_format_capacities_response const *response) {
  usb_error result = usb_execute_cbi(storage_device, (uint8_t *)&ufi_cmd_read_format_capacities, false,
                                     sizeof(ufi_format_capacities_response), (uint8_t *)response, NULL);

  RETURN_CHECK(result);
}

usb_error ufi_inquiry(device_config *const storage_device, ufi_inquiry_response const *response) {
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
  cmd.operation_code     = send ? 0x2A : 0x28;
  cmd.lba[2]             = sector_number >> 8;
  cmd.lba[3]             = sector_number & 0xFF;
  cmd.transfer_length[1] = sector_count;

  usb_error result = usb_execute_cbi(storage_device, (uint8_t *)&cmd, send, 512 * sector_count, (uint8_t *)buffer, sense_codes);

  RETURN_CHECK(result);
}

usb_error
ufi_format(device_config *const storage_device, const uint8_t side, const uint8_t track_number, const uint8_t interleave) {
  uint8_t const sense_codes[2];

  ufi_format_parameter_list parameter_list;
  memset(&parameter_list, 0, sizeof(parameter_list));

  ufi_format_command cmd;
  memcpy(&cmd, &ufi_cmd_format, sizeof(cmd));

  cmd.track_number  = track_number;
  cmd.interleave[1] = interleave;

  parameter_list.defect_list_header.side         = side;
  parameter_list.defect_list_header.immediate    = 1;
  parameter_list.defect_list_header.reserved2    = 0;
  parameter_list.defect_list_header.single_track = 1;
  parameter_list.defect_list_header.dcrt         = 0;
  parameter_list.defect_list_header.extend       = 1;
  parameter_list.defect_list_header.fov          = 0;

  usb_error result =
      usb_execute_cbi(storage_device, (uint8_t *)&cmd, TRUE, sizeof(parameter_list), (uint8_t *)&parameter_list, sense_codes);

  RETURN_CHECK(result);
}
