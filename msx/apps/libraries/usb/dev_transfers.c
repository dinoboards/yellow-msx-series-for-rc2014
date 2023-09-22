/**
 * @file transfers.c
 * @author Dean Netherton
 * @brief A simplest implementation of common usb transfer functions, based on the CH376S chip
 * @details For a basic walkthrough of the usb protocol see https://www.beyondlogic.org/usbnutshell/usb1.shtml
 * @version 1.0
 * @date 2023-09-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "dev_transfers.h"
#include "ch376.h"
#include "protocol.h"
#include <delay.h>
#include <stdlib.h>

/**
 * @brief Perform a USB control transfer (in or out)
 * See https://www.beyondlogic.org/usbnutshell/usb4.shtml for a description of the USB control transfer
 *
 * @param device the usb device
 * @param cmd_packet Pointer to the setup packet - top bit of bmRequestType indicate data direction
 * @param buffer Pointer of data to send or receive into
 * @return usb_error USB_ERR_OK if all good, otherwise specific error code
 */
usb_error usbdev_control_transfer(device_config *const device, const setup_packet *const cmd_packet, uint8_t *const buffer) {
  usb_error result;

  RETURN_CHECK(usb_control_transfer(cmd_packet, buffer, device->address, device->max_packet_size));
}

usb_error usbdev_bulk_out_transfer(device_config *const dev, uint8_t *const buffer, const uint16_t buffer_size) {

  usb_error result;

  endpoint_param *const endpoint = &dev->endpoints[ENDPOINT_BULK_OUT];

  result = usb_data_out_transfer(buffer, buffer_size, dev->address, endpoint);

  if (result == USB_ERR_STALL) {
    usb_clear_endpoint_halt(dev, ENDPOINT_BULK_OUT);
    return USB_ERR_STALL;
  }

  RETURN_CHECK(result);
}