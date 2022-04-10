#include "usb-enumerate-floppy.h"
#include "hw.h"
#include "usb-enumerate.h"
#include <string.h>

void parse_endpoint_floppy(_usb_state *const work_area, const endpoint_descriptor const *pEndpoint) {

  if (!(pEndpoint->bmAttributes & 0x02))
    return;

  if (pEndpoint->bmAttributes & 0x01) { // 3 -> Interrupt
    if (pEndpoint->bEndpointAddress & 0x80) {
      work_area->storage_device.endpoints[ENDPOINT_INTERRUPT_IN].number          = pEndpoint->bEndpointAddress & 0x07;
      work_area->storage_device.endpoints[ENDPOINT_INTERRUPT_IN].toggle          = 0;
      work_area->storage_device.endpoints[ENDPOINT_INTERRUPT_IN].max_packet_size = pEndpoint->wMaxPacketSize;
    }
  } else {
    if (pEndpoint->bEndpointAddress & 0x80) {
      work_area->storage_device.endpoints[ENDPOINT_BULK_IN].number          = pEndpoint->bEndpointAddress & 0x07;
      work_area->storage_device.endpoints[ENDPOINT_BULK_IN].toggle          = 0;
      work_area->storage_device.endpoints[ENDPOINT_BULK_IN].max_packet_size = pEndpoint->wMaxPacketSize;
    } else {
      work_area->storage_device.endpoints[ENDPOINT_BULK_OUT].number          = pEndpoint->bEndpointAddress & 0x07;
      work_area->storage_device.endpoints[ENDPOINT_BULK_OUT].toggle          = 0;
      work_area->storage_device.endpoints[ENDPOINT_BULK_OUT].max_packet_size = pEndpoint->wMaxPacketSize;
    }
  }
}
