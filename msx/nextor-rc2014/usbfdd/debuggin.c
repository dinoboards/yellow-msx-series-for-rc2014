#include "debuggin.h"
#include "print.h"
#include "usb.h"
#include "work-area.h"

void logInterface(const interface_descriptor *const p) {
  printf("len=%d,t=%d,inum=%d,alt=%d,numEnd=%d,clas=%d,sub=%d,prot=%d,int=%d\r\n",
         p->bLength,
         p->bDescriptorType,
         p->bInterfaceNumber,
         p->bAlternateSetting,
         p->bNumEndpoints,
         p->bInterfaceClass,
         p->bInterfaceSubClass,
         p->bInterfaceProtocol,
         p->iInterface);
}

void logConfig(const config_descriptor *const p) {
  printf("len=%d,typ=%d,tlen=%d,numI=%d,bcfgv=%d,icfg=%d,bmA=%d,max=%d\r\n",
         p->bLength,
         p->bDescriptorType,
         p->wTotalLength,
         p->bNumInterfaces,
         p->bConfigurationvalue,
         p->iConfiguration,
         p->bmAttributes,
         p->bMaxPower);
}

void logDevice(const device_descriptor *const p) {
  printf("len=%d,", p->bLength);
  printf("typ=%d,", p->bDescriptorType);
  printf("USB=%d,", p->bcdUSB);
  printf("cls=%d,", p->bDeviceClass);
  printf("sub=%d,", p->bDeviceSubClass);
  printf("pro=%d,", p->bDeviceProtocol);
  printf("siz=%d,", p->bMaxPacketSize0);
  printf("ven=%04X,", p->idVendor);
  printf("prd=%04X,", p->idProduct);
  printf("dev=%d,", p->bcdDevice);
  printf("man=%d,", p->iManufacturer);
  printf("ipd=%d,", p->iProduct);
  printf("ser=%d,", p->iSerialNumber);
  printf("num=%d\r\n", p->bNumConfigurations);
}

void logEndPointDescription(const endpoint_descriptor *const p) {
  printf("len=%d,t=%d,e=%d,a=%d,size=%d,intv=%d\r\n",
         p->bLength,
         p->bDescriptorType,
         p->bEndpointAddress,
         p->bmAttributes,
         p->wMaxPacketSize,
         p->bInterval);
}

void logEndPointParam(const endpoint_param *const p) {
  printf("(num=%d,tog=%d,max=%d)", p->number, p->toggle, p->max_packet_size);
}

void logWorkArea(const _usb_state *const p) {
  printf("dev: %d: max: %d, cfg: %d, int: %d, BLKOUT: ",
         p->usb_device,
         p->max_packet_size,
         p->bConfigurationvalue,
         p->interface_number);
  logEndPointParam(&p->endpoints[ENDPOINT_BULK_OUT]);
  printf(" BLKIN: ");
  logEndPointParam(&p->endpoints[ENDPOINT_BULK_IN]);
  printf(" INTIN: ");
  logEndPointParam(&p->endpoints[ENDPOINT_INTERRUPT_IN]);
  printf("\r\n");
}

// void logUsbDevice(const _usb_device_info *const info) {
//   printf("add %d, id %d, cfg: %d, max: %d, inend: %d, outend: %d, intog: %d, outog: %d\r\n",
//          info->device_address,
//          info->interface_id,
//          info->config_id,
//          info->max_packet_size,
//          info->data_bulk_in_endpoint_id,
//          info->data_bulk_out_endpoint_id,
//          info->data_bulk_in_endpoint_toggle,
//          info->data_bulk_out_endpoint_toggle);
// }
