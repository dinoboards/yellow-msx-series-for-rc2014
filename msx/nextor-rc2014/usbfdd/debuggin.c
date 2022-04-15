#include "debuggin.h"
#include "nextor.h"
#include "print.h"
#include "usb.h"
#include "work-area.h"
#include <string.h>

// void logInterface(const interface_descriptor *const p) {
//   printf("int len=%d,t=%d,inum=%d,alt=%d,numEnd=%d,clas=%d,sub=%d,prot=%d,int=%d\r\n",
//          p->bLength,
//          p->bDescriptorType,
//          p->bInterfaceNumber,
//          p->bAlternateSetting,
//          p->bNumEndpoints,
//          p->bInterfaceClass,
//          p->bInterfaceSubClass,
//          p->bInterfaceProtocol,
//          p->iInterface);
// }

// void logConfig(const config_descriptor *const p) {
//   printf("cfg len=%d,typ=%d,tlen=%d,numI=%d,bcfgv=%d,icfg=%d,bmA=%d,max=%d\r\n",
//          p->bLength,
//          p->bDescriptorType,
//          p->wTotalLength,
//          p->bNumInterfaces,
//          p->bConfigurationvalue,
//          p->iConfiguration,
//          p->bmAttributes,
//          p->bMaxPower);
// }

void logDevice(const device_descriptor *const p) {
  printf("dev len=%d,", p->bLength);
  printf("typ=%d,", p->bDescriptorType);
  printf("USB=%02X,", p->bcdUSB);
  // printf("cls=%02x,", p->bDeviceClass);
  // printf("sub=%02x,", p->bDeviceSubClass);
  // printf("pro=%02x,", p->bDeviceProtocol);
  printf("siz=%d,", p->bMaxPacketSize0);
  // printf("ven=%04X,", p->idVendor);
  // printf("prd=%04X,", p->idProduct);
  // printf("dev=%d,", p->bcdDevice);
  // printf("man=%d,", p->iManufacturer);
  // printf("ipd=%d,", p->iProduct);
  // printf("ser=%d,", p->iSerialNumber);
  printf("num=%d\r\n", p->bNumConfigurations);
}

// void logEndPointDescription(const endpoint_descriptor *const p) {
//   printf("ep: len=%d,t=%d,e=%d,a=%d,size=%d,intv=%d\r\n",
//          p->bLength,
//          p->bDescriptorType,
//          p->bEndpointAddress,
//          p->bmAttributes,
//          p->wMaxPacketSize,
//          p->bInterval);
// }

// void logEndPointParam(const endpoint_param *const p) {
//   printf("enp (num=%d,tog=%d,max=%d)", p->number, p->toggle, p->max_packet_size);
// }

// void logWorkArea(const _usb_state *const p) {
//   printf("dev: %d: h-max: %d, h-cfg: %d, f-max: %d, f-cfg: %d, int: %d, BLKOUT: ",
//          p->usb_device,
//          p->hub_config.max_packet_size,
//          p->hub_config.value,
//          p->floppy_config.max_packet_size,
//          p->floppy_config.value,
//          p->interface_number);
//   logEndPointParam(&p->endpoints[ENDPOINT_BULK_OUT]);
//   printf(" BLKIN: ");
//   logEndPointParam(&p->endpoints[ENDPOINT_BULK_IN]);
//   printf(" INTIN: ");
//   logEndPointParam(&p->endpoints[ENDPOINT_INTERRUPT_IN]);
//   printf("\r\n");
// }

// void logInquiryResponse(const ufi_inquiry_response *const response) {
//   printf("typ: %d,", response->device_type);
//   printf("rmb: %d", response->removable_media);
//   printf("ans: %d,", response->ansi_version);
//   printf("ecma: %d,", response->ecma);
//   printf("iso: %d,", response->iso_version);
//   printf("fmt: %d,", response->response_data_format);
//   printf("add: %d,", response->additional_length);

//   char buffer[20];

//   memset(buffer, 0, 20);
//   memcpy(buffer, response->product_id, 16);
//   print_string(buffer);
//   print_string(",");

//   memset(buffer, 0, 20);
//   memcpy(buffer, response->vendor_information, 8);
//   print_string(buffer);
//   print_string(",");

//   memset(buffer, 0, 20);
//   memcpy(buffer, response->product_revision, 4);
//   print_string(buffer);
//   print_string("\r\n");
// }

// void logSetupPacket(const setup_packet *const cmd_packet) {
//   printf("bmRequestType = %d", cmd_packet->bmRequestType);
//   printf("bRequest = %d", cmd_packet->bRequest);
//   printf("bValue = %d,%d", cmd_packet->bValue[0], cmd_packet->bValue[1]);
//   printf("bIndex = %d,%d", cmd_packet->bIndex[0], cmd_packet->bIndex[1]);
//   printf("wLength = %d\r\n", cmd_packet->wLength);
// }

// void logNextorLunInfo(const nextor_lun_info* const info) {
//   printf("medium_type = %d,", info->medium_type);
//   printf("sector_size = %d,", info->sector_size);
//   printf("number_of_sectors = %ld,", info->number_of_sectors);
//   printf("flags = %02x", info->flags);
//   printf("number_of_cylinders = %d,", info->number_of_cylinders);
//   printf("number_of_heads = %d,", info->number_of_heads);
//   printf("number_of_sectors_per_track = %d,", info->number_of_sectors_per_track);
// }
