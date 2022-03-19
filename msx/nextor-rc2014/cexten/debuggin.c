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