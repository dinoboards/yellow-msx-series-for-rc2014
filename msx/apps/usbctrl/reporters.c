#include "reporters.h"
#include <io.h>
#include <stdio.h>
#include <string.h>

void report_device_descriptor(const device_descriptor *const p, FILE *file_handle) __sdcccall(1) {
  fprintf(file_handle, "Device:\r");
  fprintf(file_handle, "  length:             %d\r", p->bLength);
  fprintf(file_handle, "  bDescriptorType:    %d\r", p->bDescriptorType);
  fprintf(file_handle, "  bcdUSB:             %02X\r", p->bcdUSB);
  fprintf(file_handle, "  bDeviceClass:       %02x\r", p->bDeviceClass);
  fprintf(file_handle, "  bDeviceSubClass:    %02x\r", p->bDeviceSubClass);
  fprintf(file_handle, "  bDeviceProtocol:    %02x\r", p->bDeviceProtocol);
  fprintf(file_handle, "  bMaxPacketSize0:    %d\r", p->bMaxPacketSize0);
  fprintf(file_handle, "  idVendor:           %04X\r", p->idVendor);
  fprintf(file_handle, "  idProduct:          %04X\r", p->idProduct);
  fprintf(file_handle, "  bcdDevice:          %04X\r", p->bcdDevice);
  fprintf(file_handle, "  iManufacturer:      %d\r", p->iManufacturer);
  fprintf(file_handle, "  iProduct:           %d\r", p->iProduct);
  fprintf(file_handle, "  iSerialNumber:      %d\r", p->iSerialNumber);
  fprintf(file_handle, "  bNumConfigurations: %d\r", p->bNumConfigurations);
}
