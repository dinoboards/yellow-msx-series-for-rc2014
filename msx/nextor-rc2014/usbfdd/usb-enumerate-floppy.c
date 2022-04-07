#include "usb-enumerate-floppy.h"
#include "hw.h"
#include "usb-enumerate.h"
#include <string.h>

void parse_endpoint_floppy(_usb_state *const work_area, const endpoint_descriptor const *pEndpoint) {

  if (!(pEndpoint->bmAttributes & 0x02))
    return;

  if (pEndpoint->bmAttributes & 0x01) { // 3 -> Interrupt
    if (pEndpoint->bEndpointAddress & 0x80) {
      work_area->endpoints[ENDPOINT_INTERRUPT_IN].number          = pEndpoint->bEndpointAddress & 0x07;
      work_area->endpoints[ENDPOINT_INTERRUPT_IN].toggle          = 0;
      work_area->endpoints[ENDPOINT_INTERRUPT_IN].max_packet_size = pEndpoint->wMaxPacketSize;
    }
  } else {
    if (pEndpoint->bEndpointAddress & 0x80) {
      work_area->endpoints[ENDPOINT_BULK_IN].number          = pEndpoint->bEndpointAddress & 0x07;
      work_area->endpoints[ENDPOINT_BULK_IN].toggle          = 0;
      work_area->endpoints[ENDPOINT_BULK_IN].max_packet_size = pEndpoint->wMaxPacketSize;
    } else {
      work_area->endpoints[ENDPOINT_BULK_OUT].number          = pEndpoint->bEndpointAddress & 0x07;
      work_area->endpoints[ENDPOINT_BULK_OUT].toggle          = 0;
      work_area->endpoints[ENDPOINT_BULK_OUT].max_packet_size = pEndpoint->wMaxPacketSize;
    }
  }
}

void configure_usb_floppy(_usb_state *const work_area) {
  usb_error result;

  if ((result = hw_set_address(DEVICE_ADDRESS_FLOPPY, work_area->max_packet_size)) != USB_ERR_OK) {
    // yprintf(15, "X2 (%d)", result);
    return;
  }

  if ((result = usb_set_configuration(work_area->bConfigurationvalue, work_area->max_packet_size, DEVICE_ADDRESS_FLOPPY)) !=
      USB_ERR_OK) {
    // yprintf(15, "X3 (%d)", result);
    return;
  }
}
