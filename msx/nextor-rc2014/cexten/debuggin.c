#include "print.h"
#include "work-area.h"
#include "debuggin.h"

void logConfig(const config_descriptor* const p) {
  printf("len%d,typ%d,tlen%d,num%d,bcfgv%d,icfg%d,bmA%d,max%d",
    p->bLength,
    p->bDescriptorType,
    p->wTotalLength,
    p->bNumInterfaces,
    p->bConfigurationvalue,
    p->iConfiguration,
    p->bmAttributes,
    p->bMaxPower
  );
}

void logDevice(const device_descriptor* const p) {
  printf(
    "len=%d,typ=%d,usb=%d,clas=%d,sub=%d,pro=%d,size=%d,ven=%d,prod=%d,dev=%d,man=%d,prd=%d,ser=%d,num=%d",
    p->bLength,
    p->bDescriptorType,
    p->bcdUSB,
    p->bDeviceClass,
    p->bDeviceSubClass,
    p->bDeviceProtocol,
    p->bMaxPacketSize0,
    p->idVendor,
    p->idProduct,
    p->bcdDevice,
    p->iManufacturer,
    p->iProduct,
    p->iSerialNumber,
    p->bNumConfigurations
  );
}

void logUsbDevice(const _usb_device_info* const info) {
  printf(
    "add %d, id %d, cfg: %d, max: %d, inend: %d, outend: %d, intog: %d, outog: %d\r\n",
    info->device_address,
    info->interface_id,
    info->config_id,
    info->max_packet_size,
    info->data_bulk_in_endpoint_id,
    info->data_bulk_out_endpoint_id,
    info->data_bulk_in_endpoint_toggle,
    info->data_bulk_out_endpoint_toggle
  );
}
