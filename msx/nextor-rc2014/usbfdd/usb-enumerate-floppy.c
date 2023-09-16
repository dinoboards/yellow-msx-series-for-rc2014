#include "usb-enumerate-floppy.h"
#include "hw.h"
#include "usb-enumerate.h"
#include <string.h>

void parse_endpoint_storage(storage_device_config *const storage_dev, const endpoint_descriptor const *pEndpoint) {

  if (!(pEndpoint->bmAttributes & 0x02))
    return;

  if (pEndpoint->bmAttributes & 0x01) { // 3 -> Interrupt
    if (pEndpoint->bEndpointAddress & 0x80) {
      storage_dev->endpoints[ENDPOINT_INTERRUPT_IN].number          = pEndpoint->bEndpointAddress & 0x07;
      storage_dev->endpoints[ENDPOINT_INTERRUPT_IN].toggle          = 0;
      storage_dev->endpoints[ENDPOINT_INTERRUPT_IN].max_packet_size = pEndpoint->wMaxPacketSize;
    }
  } else {
    if (pEndpoint->bEndpointAddress & 0x80) {
      storage_dev->endpoints[ENDPOINT_BULK_IN].number          = pEndpoint->bEndpointAddress & 0x07;
      storage_dev->endpoints[ENDPOINT_BULK_IN].toggle          = 0;
      storage_dev->endpoints[ENDPOINT_BULK_IN].max_packet_size = pEndpoint->wMaxPacketSize;
    } else {
      storage_dev->endpoints[ENDPOINT_BULK_OUT].number          = pEndpoint->bEndpointAddress & 0x07;
      storage_dev->endpoints[ENDPOINT_BULK_OUT].toggle          = 0;
      storage_dev->endpoints[ENDPOINT_BULK_OUT].max_packet_size = pEndpoint->wMaxPacketSize;
    }
  }
}
