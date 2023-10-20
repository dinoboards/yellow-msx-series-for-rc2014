#include "usb_cbi.h"
#include <ch376.h>
#include <dev_transfers.h>
#include <protocol.h>
#include <stdbool.h>

setup_packet cbi2_adsc = {0x21, 0, {0, 0}, {255, 0}, 12}; // ;4th byte is interface number

// was no clear
usb_error usb_execute_cbi(device_config *const storage_device,
                          const uint8_t *const cmd,
                          const bool           send,
                          const uint16_t       buffer_size,
                          uint8_t *const       buffer,
                          uint8_t *const       sense_codes) {

  usb_error result;

  const uint8_t interface_number = storage_device->interface_number;

  setup_packet adsc;
  adsc           = cbi2_adsc;
  adsc.bIndex[0] = interface_number;

  result = usbdev_control_transfer(storage_device, &adsc, (uint8_t *const)cmd);

  if (result == USB_ERR_STALL) {
    if (sense_codes != NULL)
      usbdev_data_in_transfer(storage_device, sense_codes, 2, ENDPOINT_INTERRUPT_IN);

    return USB_ERR_STALL;
  }

  if (result != USB_ERR_OK) {
    TRACE_USB_ERROR(result);
    return result;
  }

  if (send) {
    result = usbdev_bulk_out_transfer(storage_device, buffer, buffer_size);

    if (result != USB_ERR_OK) {
      TRACE_USB_ERROR(result);
      return result;
    }
  } else {
    result = usbdev_data_in_transfer(storage_device, buffer, buffer_size, ENDPOINT_BULK_IN);

    if (result != USB_ERR_OK) {
      TRACE_USB_ERROR(result);
      return result;
    }
  }

  if (sense_codes != NULL) {
    result = usbdev_data_in_transfer(storage_device, sense_codes, 2, ENDPOINT_INTERRUPT_IN);

    if (result != USB_ERR_OK) {
      TRACE_USB_ERROR(result);
      return result;
    }
  }

  return USB_ERR_OK;
}
