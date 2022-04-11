#include "ch376.h"
#include "hw.h"
#include "nextor.h"
#include "scsi.h"
#include "usb-enumerate-hub.h"
#include "usb-enumerate.h"
#include "usb.h"
#include "work-area.h"
#include <delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "debuggin.h"
#include "print.h"

usb_error usb_host_bus_reset() {
  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60 / 4);

  ch376_set_usb_mode(CH_MODE_HOST_RESET);
  delay(60 / 2);

  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60 / 4);

  ch_configure_nak_retry_indefinite();

  return USB_ERR_OK;
}

void state_devices(const _usb_state *const work_area) {
  if (work_area->xusb_device == 0)
    return;

  print_string("USB:             ");

  usb_device_type dev = work_area->xusb_device;

  if (dev & USB_IS_HUB) {
    print_string("HUB");

    dev &= ~USB_IS_HUB;
    if (dev)
      print_string(", ");
  }

  if (dev & USB_IS_FLOPPY) {
    print_string("FLOPPY");

    dev &= ~USB_IS_FLOPPY;
    if (dev)
      print_string(", ");
  }

  if (dev & USB_IS_MASS_STORAGE)
    print_string("STORAGE");

  print_string("\r\n");
}

uint8_t usb_host_init() {
  work_area *const p = get_work_area();
  __asm__("EI");
  _usb_state *const work_area = &p->ch376;
  memset(work_area, 0, sizeof(_usb_state));

  ch376_reset();

  if (!ch376_probe()) {
    print_string("CH376:           NOT PRESENT\r\n");
    return false;
  }

  const uint8_t ver = ch376_get_firmware_version();
  print_string("CH376:           PRESENT (VER ");
  print_hex(ver);
  print_string(")\r\n");

  usb_host_bus_reset();
  delay(10);

  enumerate_all_devices();

  state_devices(work_area);

  // logWorkArea(work_area);

  // usb_error result;
  // // result = get_scsi_max_luns();
  // // printf("max-luns %d\r\n", result);

  // scsi_inquiry_result inq_result;

  // result = get_scsi_inquiry(&inq_result);

  // if (result == USB_ERR_OK) {
  //   uint8_t buffer[10];
  //   memset(buffer, 0, 10);
  //   memcpy(buffer, &inq_result.buffer[8], 8);

  //   printf("Ven: %s\r\n", buffer);
  // }

  // wait_for_mounting();

  // scsi_read_capacity_result cap;

  // result = get_scsi_read_capacity(&cap);

  // uint32_t number_of_blocks;
  // uint8_t *n = (uint8_t *)&number_of_blocks;
  // *n++       = cap.number_of_blocks[3];
  // *n++       = cap.number_of_blocks[2];
  // *n++       = cap.number_of_blocks[1];
  // *n         = cap.number_of_blocks[0];

  // printf("r(%d), number of blks %ld\r\n", result, number_of_blocks);

  // ufi_format_capacities_response response;
  // memset(&response, 0, sizeof(ufi_format_capacities_response));

  // result = scsi_capacity(work_area, &response);
  // printf("CAP result: %d, %d\r\n", result, response.block_size[1]);

  // ufi_inquiry_response resp;
  // memset(&resp, 0, sizeof(ufi_inquiry_response));
  // result = ufi_inquiry(&work_area->storage_device[0], &resp);
  // if (result == USB_ERR_OK)
  //   logInquiryResponse(&resp);
  // else
  //   printf("Err %d\r\n", result);

  // uint8_t buffer[512];
  // result = ufi_read_sector(work_area, 2, buffer);
  // printf("read %d\r\n", result);

  return work_area->xusb_device != USB_IS_HUB && work_area->xusb_device != 0;
}
