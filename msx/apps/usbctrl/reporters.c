#include "reporters.h"
#include "rotator.h"
#include <io.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void print_with_progress(FILE *file_handle, const char *const fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(file_handle, fmt, args);
  va_end(args);
  rotate();
}

void report_device_descriptor(const device_descriptor *const p, FILE *file_handle) __sdcccall(1) {
  print_with_progress(file_handle, "Device:\r");
  print_with_progress(file_handle, "  length:             %d\r", p->bLength);
  print_with_progress(file_handle, "  bDescriptorType:    %d\r", p->bDescriptorType);
  print_with_progress(file_handle, "  bcdUSB:             %02X\r", p->bcdUSB);
  print_with_progress(file_handle, "  bDeviceClass:       %02x\r", p->bDeviceClass);
  print_with_progress(file_handle, "  bDeviceSubClass:    %02x\r", p->bDeviceSubClass);
  print_with_progress(file_handle, "  bDeviceProtocol:    %02x\r", p->bDeviceProtocol);
  print_with_progress(file_handle, "  bMaxPacketSize0:    %d\r", p->bMaxPacketSize0);
  print_with_progress(file_handle, "  idVendor:           %04X\r", p->idVendor);
  print_with_progress(file_handle, "  idProduct:          %04X\r", p->idProduct);
  print_with_progress(file_handle, "  bcdDevice:          %04X\r", p->bcdDevice);
  print_with_progress(file_handle, "  iManufacturer:      %d\r", p->iManufacturer);
  print_with_progress(file_handle, "  iProduct:           %d\r", p->iProduct);
  print_with_progress(file_handle, "  iSerialNumber:      %d\r", p->iSerialNumber);
  print_with_progress(file_handle, "  bNumConfigurations: %d\r", p->bNumConfigurations);
}

void report_device_configuration(const config_descriptor *const config, FILE *file_handle) __sdcccall(1) {
  print_with_progress(file_handle, "  Configuration:\r");
  print_with_progress(file_handle, "    length:              %d\r", config->bLength);
  print_with_progress(file_handle, "    bDescriptorType:     %d\r", config->bDescriptorType);
  print_with_progress(file_handle, "    wTotalLength:        %d\r", config->wTotalLength);
  print_with_progress(file_handle, "    bNumInterfaces:      %d\r", config->bNumInterfaces);
  print_with_progress(file_handle, "    bConfigurationvalue: %d\r", config->bConfigurationvalue);
  print_with_progress(file_handle, "    iConfiguration:      %d\r", config->iConfiguration);
  print_with_progress(file_handle, "    bmAttributes:        %d\r", config->bmAttributes);
  print_with_progress(file_handle, "    bMaxPower:           %d\r", config->bMaxPower);
}

void report_device_interface(const interface_descriptor *const interface, FILE *file_handle) __sdcccall(1) {
  print_with_progress(file_handle, "    Interface:\r");
  print_with_progress(file_handle, "      bLength:            %d\r", interface->bLength);
  print_with_progress(file_handle, "      bDescriptorType:    %d\r", interface->bDescriptorType);
  print_with_progress(file_handle, "      bInterfaceNumber:   %d\r", interface->bInterfaceNumber);
  print_with_progress(file_handle, "      bAlternateSetting:  %d\r", interface->bAlternateSetting);
  print_with_progress(file_handle, "      bNumEndpoints:      %d\r", interface->bNumEndpoints);
  print_with_progress(file_handle, "      bInterfaceClass:    %d\r", interface->bInterfaceClass);
  print_with_progress(file_handle, "      bInterfaceSubClass: %d\r", interface->bInterfaceSubClass);
  print_with_progress(file_handle, "      bInterfaceProtocol: %d\r", interface->bInterfaceProtocol);
  print_with_progress(file_handle, "      iInterface:         %d\r", interface->iInterface);
}
