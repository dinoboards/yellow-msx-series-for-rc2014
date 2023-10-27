#include "main.h"
#include "print.h"
// #include "printer_drv.h"
#include "usb-dev-info-ufi.h"
#include "usb-dev-read-ufi.h"
#include "usb-dev-write-ufi.h"
#include "usb-lun-info-ufi.h"
#include "usb_state.h"
#include <ch376.h>
#include <class_hid.h>
#include <class_hid_keyboard.h>
#include <class_scsi.h>
#include <class_ufi.h>
#include <delay.h>
#include <enumerate.h>
#include <msxdos.h>
#include <usb/vendor_ftdi.h>
#include <usb_trace.h>

uint16_t dumpSector(device_config *config, const uint32_t number) {
  usb_error error;

  uint8_t buffer[512];
  memset(buffer, 0, sizeof(buffer));
  uint8_t read = 0;

  printf("Sector %d\r\n", number);
  if ((error = usb_dev_read_ufi(config, 1, 1, number, buffer, &read)) != 0) {
    printf("usb_dev_read_ufi: %d, %d\n", error, read);
    return error;
  }

  for (int i = 0; i < 32; i++) {
    printf("%02X ", buffer[i]);
    if (i % 16 == 15)
      printf("\n");
  }
  return 0;
}

uint8_t writeSector(device_config *config, const uint32_t number) {
  usb_error error;

  uint8_t buffer[512];
  uint8_t written = 0;
  memset(buffer, 0, sizeof(buffer));
  buffer[3] = 0xF6;
  buffer[4] = number + 0x60;

  error = usb_dev_write_ufi(config, 1, 1, number, buffer, &written);
  printf("usb_dev_write_ufi: %d\n", error);
  return error;
}

void chput(const char c) __z88dk_fastcall { printf("%c", c); }

usb_error usb_host_bus_reset(void) {
  ch_cmd_set_usb_mode(CH_MODE_HOST);
  delay_short();

  ch_cmd_set_usb_mode(CH_MODE_HOST_RESET);
  delay_short();

  ch_cmd_set_usb_mode(CH_MODE_HOST);
  delay_short();

  ch_configure_nak_retry_3s();

  return USB_ERR_OK;
}

void right_trim(char *buffer) {
  for (int i = strlen(buffer) - 1; i >= 0; i--)
    if (buffer[i] == ' ')
      buffer[i] = 0;
    else
      break;
}

const char *printer_test_txt = "Verifying printer output works!\n";

#define BUF_SIZE 64

usb_error read_from_ftdi(device_config_ftdi *ftdi_config, uint8_t *const read_bytes) {
  usb_error result;
  uint8_t   buffer[BUF_SIZE];
  uint8_t   buffer_size = BUF_SIZE;

  memset(buffer, 0, sizeof(buffer));

  result      = ftdi_read_data(ftdi_config, buffer, &buffer_size);
  *read_bytes = buffer_size;
  // printf(" (%d, %d) \r\n", buffer_size, *read_count);

  if (result)
    return result;

  for (uint8_t i = 0; i < buffer_size; i++)
    printf("%02X  ", buffer[i]);

  return USB_ERR_OK;
}

void conduct_ftdi_loop_back_verification(void) {
  usb_error           result;
  device_config_ftdi *ftdi_config = (device_config_ftdi *)find_device_config(USB_IS_FTDI);

  uint32_t baud_rate = 4800;
  printf("ftdi_set_baudrate (requested: %ld", baud_rate);
  result = ftdi_set_baudrate(ftdi_config, &baud_rate);
  if (result)
    printf(")\r\nresult: %d\r\n", result);

  printf(", %ld)\r\n", baud_rate);

  printf("ftdi_set_line_property2: 8 bits, 1 stop bit, no parity, break off\r\n");
  result = ftdi_set_line_property2(ftdi_config, BITS_8, STOP_BIT_1, NONE, BREAK_OFF);
  if (result)
    printf("result: %d\r\n", result);

  printf("ftdi_usb_purge_tx/rx_buffer\r\n");
  result = ftdi_usb_purge_tx_buffer(ftdi_config);
  if (result)
    printf("ftdi_usb_purge_tx_buffer failed: %d\r\n", result);

  result = ftdi_usb_purge_rx_buffer(ftdi_config);
  if (result)
    printf("ftdi_usb_purge_rx_buffer failed: %d\r\n", result);

  uint8_t buffer_size = BUF_SIZE;
  uint8_t buffer[BUF_SIZE];
  uint8_t id = 0;

  while (true) {
    if (msxbiosBreakX())
      goto finally;

    printf("ftdi_write_data (64 bytes, from %d)\r\n", id);

    for (uint8_t buf_index = 0; buf_index < BUF_SIZE; buf_index++)
      buffer[buf_index] = id++;

    result = ftdi_write_data(ftdi_config, buffer, BUF_SIZE);
    if (result) {
      printf("ftdi_write_data errored: %d\r\n", result);
      break;
    }

    printf("ftdi_read_data: \r\n");
    uint8_t  read_count;
    uint8_t  total_read = 0;
    uint16_t count      = 0;
    int16_t  timeout    = get_future_time(from_ms(5000));
    while (total_read != 64 && !is_time_past(timeout)) {
      if (msxbiosBreakX())
        goto finally;

      result = read_from_ftdi(ftdi_config, &read_count);
      total_read += read_count;
      count++;
      if (result) {
        printf("ftdi_read_data errored: %d\r\n", result);
        break;
      }
    }

    if (total_read != 64) {
      printf("\r\nftdi_read_data returned %d bytes in %d chunks.  Expected 64 bytes\r\n", total_read, count);
      continue;
    }

    do {
      if (msxbiosBreakX())
        goto finally;

      result = read_from_ftdi(ftdi_config, &read_count);
      if (result) {
        printf("ftdi_read_data errored: %d\r\n", result);
        break;
      }

      if (read_count != 0)
        printf("\r\nftdi_read_data returned %d bytes.  Expected 0 bytes\r\n", read_count);
    } while (read_count != 0);
  }

finally:
}

void state_devices(_usb_state *const work_area) __z88dk_fastcall {

  const bool hasCdc      = find_device_config(USB_IS_CDC) != NULL;
  const bool hasUnknown  = find_device_config(USB_IS_UNKNOWN) != NULL;
  const bool hasKeyboard = find_device_config(USB_IS_KEYBOARD) != NULL;
  const bool hasFTDI     = find_device_config(USB_IS_FTDI) != NULL;

  uint8_t   index = MAX_NUMBER_OF_STORAGE_DEVICES;
  usb_error result;

  uint8_t buffer[512];

  print_string("USB: (%d)\r\n", work_area->count_of_detected_usb_devices);

  if (hasFTDI) {
    printf("FTDI\r\n");

    conduct_ftdi_loop_back_verification();
  }

  if (hasCdc)
    print_string("CDC\r\n");

  if (hasUnknown) {
    print_string("UNKNOWN\r\n");
  }

  if (hasKeyboard) {
    const device_config_keyboard *const keyboard_config = (device_config_keyboard *)find_device_config(USB_IS_KEYBOARD);
    printf("Keyboard detected\r\n");

    printf("  address: %d\r\n", keyboard_config->address);
    printf("  max_packet_size: %d\r\n", keyboard_config->max_packet_size);
    printf("  interface_number: %d\r\n", keyboard_config->interface_number);
    printf("  toggle: %d\r\n", keyboard_config->endpoints[0].toggle);
    printf("  number: %d\r\n", keyboard_config->endpoints[0].number);
    printf("  max packet sizeX: %d\r\n", keyboard_config->endpoints[0].max_packet_sizex);

    result = hid_set_protocol(keyboard_config, 1);
    printf("  hid_set_protocol: %d\r\n", result);

    result = hid_set_idle(keyboard_config, 0x80);
    printf("  hid_set_idle: %d\r\n", result);

    memset(buffer, 0, sizeof(buffer));
    const keyboard_report *const p_report = (const keyboard_report *)buffer;

    while (!msxbiosBreakX()) {
      for (uint16_t i = 0; i < 1000; i++) {
        ch_configure_nak_retry_disable();
        result = usbdev_data_in_transfer_ep0((device_config *)keyboard_config, buffer, 8);
        ch_configure_nak_retry_3s();
        if (result == 0) {
          const char c = scancode_to_char(p_report->bModifierKeys, p_report->keyCode[0]);
          if (c >= 32 && c < 127)
            printf("%c: ", c);
          else
            printf("0x%02X: ", p_report->keyCode[0]);

          for (uint8_t i = 0; i < 8; i++)
            printf(" %02X ", buffer[i]);

          printf("\r\n");
        }
      }
      delay(3);
    }
  }

  for (uint8_t index = 1; index <= MAX_NUMBER_OF_STORAGE_DEVICES; index++) {
    device_config        *storage_device = get_usb_device_config(index);
    const usb_device_type t              = storage_device->type;

    memset(buffer, 0, sizeof(buffer));

    if (t == USB_IS_FLOPPY) {
      ufi_format_capacities_response r;
      memset(&r, 0, sizeof(r));

      result = wait_for_device_ready(storage_device, 2500);
      printf("wait_for_device_ready: %d\r\n", result);

      printf("--\r\n");

      result = ufi_read_format_capacities(storage_device, &r);
      printf("ufi_read_format_capacities: %d,", result);
      log_ufi_format_capacities_response(&r);

    } else if (t == USB_IS_MASS_STORAGE) {
      print_string("SCSI\r\n  VENDOR NAME: ");

      scsi_inquiry_result response;
      memset(&response, 0, sizeof(scsi_inquiry_result));
      result = scsi_inquiry((device_config *const)storage_device, &response);

      memcpy(buffer, response.vendor_information, 8);
      right_trim(buffer);
      print_string(buffer);
      print_string("\r\n");

      scsi_read_capacity_result scsi_response;
      get_scsi_read_capacity((device_config *const)storage_device, &scsi_response);

      uint16_t sector_size = scsi_response.block_size[2] << 8 + scsi_response.block_size[3];
      uint32_t number_of_sectors;

      uint8_t       *no_sectors = ((uint8_t *)&number_of_sectors);
      const uint8_t *no_blocks  = scsi_response.number_of_blocks + 3;

      *no_sectors++ = *no_blocks--;
      *no_sectors++ = *no_blocks--;
      *no_sectors++ = *no_blocks--;
      *no_sectors   = *no_blocks--;

      printf("  SECTOR_SIZE: %d\r\n  NUMBER_OF_SECTORS: %ld\r\n", sector_size, number_of_sectors);

      result = scsi_read_write((device_config *)storage_device, false, 0, 1, buffer);
      printf("  scsi_read_write: %d\r\n", result);
    }
  };
}

void state_storage_order(void) {
  uint8_t index = 1; // MAX_NUMBER_OF_STORAGE_DEVICES;

  do {
    const device_config *const storage_device = get_usb_device_config(index);
    if (storage_device == NULL)
      break;

    const usb_device_type t = storage_device->type;
    if (t == USB_IS_FLOPPY) {
      print_string("    FLOPPY\r\n");

    } else if (t == USB_IS_MASS_STORAGE) {
      print_string("    STORAGE\r\n");
    }

  } while (++index != MAX_NUMBER_OF_STORAGE_DEVICES + 1);
}

void initialise_scsi_devices(void) {
  const _usb_state *const boot_state = get_usb_boot_area();

  for (device_config *storage_device = first_device_config(boot_state); storage_device;
       storage_device                = next_device_config(boot_state, storage_device))
    if (storage_device->type == USB_IS_MASS_STORAGE) {
                     const usb_error result = scsi_sense_init(storage_device);
                     if (result)
        printf("\r\nscsi_sense_init: %d\r\n", result);
    }
}

#define ERASE_LINE "\x1B\x6C\r"

uint16_t simulate_boot_phase_1(void) {
  _usb_state *const boot_state = get_usb_boot_area();
  memset(boot_state, 0, sizeof(_usb_state));

  ch_cmd_reset_all();

  if (!ch_probe()) {
    print_string("CH376:           NOT PRESENT\r\n");
    goto finally;
  }

  boot_state->connected = true;

  print_string("CH376:           PRESENT (VER ");
  print_hex(ch_cmd_get_ic_version());
  print_string(")\r\n");

  print_string("USB:             SCANNING...");

  usb_host_bus_reset();

  for (uint8_t i = 0; i < 4; i++) {
    const uint8_t r = ch_very_short_wait_int_and_get_status();

    if (r == USB_INT_CONNECT) {
      enumerate_all_devices();

      initialise_scsi_devices();

      goto finally;
    }
  }

finally:
  print_string(ERASE_LINE);

  const uint16_t last = (uint16_t)find_first_free();

  boot_state->bytes_required = (last - (uint16_t)boot_state) + 1;

  return boot_state->bytes_required;
}

void simulate_boot_phase_2(const uint16_t size) {
  (void)size;
  _usb_state *const boot_state = get_usb_boot_area();
  _usb_state *const usb_state  = get_usb_work_area();
  __asm__("EI");
  memcpy(usb_state, boot_state, boot_state->bytes_required);

  if (!boot_state->connected)
    return;

  state_devices(usb_state);

  state_storage_order();
}

void main(const int argc, const char *argv[]) {
  (void)argc;
  (void)argv;

  const uint16_t size = simulate_boot_phase_1();
  printf("memory size %d\r\n", size);

  simulate_boot_phase_2(size);
}
