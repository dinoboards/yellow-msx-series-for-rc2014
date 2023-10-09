#include "reporters.h"
#include "rotator.h"
#include <fusion_ext.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void report_device_descriptor(const device_descriptor *const p, const int file_handle) __sdcccall(1) {
  fusion_fprintf(file_handle, "Device:\r\n");
  fusion_fprintf(file_handle, "  length:             %d\r\n", p->bLength);
  fusion_fprintf(file_handle, "  bDescriptorType:    %d\r\n", p->bDescriptorType);
  fusion_fprintf(file_handle, "  bcdUSB:             0x%02X\r\n", p->bcdUSB);
  fusion_fprintf(file_handle, "  bDeviceClass:       0x%02X\r\n", p->bDeviceClass);
  fusion_fprintf(file_handle, "  bDeviceSubClass:    0x%02X\r\n", p->bDeviceSubClass);
  fusion_fprintf(file_handle, "  bDeviceProtocol:    0x%02X\r\n", p->bDeviceProtocol);
  fusion_fprintf(file_handle, "  bMaxPacketSize0:    %d\r\n", p->bMaxPacketSize0);
  fusion_fprintf(file_handle, "  idVendor:           0x%04X\r\n", p->idVendor);
  fusion_fprintf(file_handle, "  idProduct:          0x%04X\r\n", p->idProduct);
  fusion_fprintf(file_handle, "  bcdDevice:          0x%04X\r\n", p->bcdDevice);
  fusion_fprintf(file_handle, "  iManufacturer:      %d\r\n", p->iManufacturer);
  fusion_fprintf(file_handle, "  iProduct:           %d\r\n", p->iProduct);
  fusion_fprintf(file_handle, "  iSerialNumber:      %d\r\n", p->iSerialNumber);
  fusion_fprintf(file_handle, "  bNumConfigurations: %d\r\n", p->bNumConfigurations);

  rotate();
}

void report_device_configuration(const config_descriptor *const config, const int file_handle) __sdcccall(1) {
  fusion_fprintf(file_handle, "  Configuration:\r\n");
  fusion_fprintf(file_handle, "    length:              %d\r\n", config->bLength);
  fusion_fprintf(file_handle, "    bDescriptorType:     %d\r\n", config->bDescriptorType);
  fusion_fprintf(file_handle, "    wTotalLength:        %d\r\n", config->wTotalLength);
  fusion_fprintf(file_handle, "    bNumInterfaces:      %d\r\n", config->bNumInterfaces);
  fusion_fprintf(file_handle, "    bConfigurationvalue: %d\r\n", config->bConfigurationvalue);
  fusion_fprintf(file_handle, "    iConfiguration:      %d\r\n", config->iConfiguration);
  fusion_fprintf(file_handle, "    bmAttributes:        0x%02X\r\n", config->bmAttributes);
  fusion_fprintf(file_handle, "    bMaxPower:           %d\r\n", config->bMaxPower);

  rotate();
}

void report_device_interface(const interface_descriptor *const interface, const int file_handle) __sdcccall(1) {
  fusion_fprintf(file_handle, "    Interface:\r\n");
  fusion_fprintf(file_handle, "      bLength:            %d\r\n", interface->bLength);
  fusion_fprintf(file_handle, "      bDescriptorType:    %d\r\n", interface->bDescriptorType);
  fusion_fprintf(file_handle, "      bInterfaceNumber:   %d\r\n", interface->bInterfaceNumber);
  fusion_fprintf(file_handle, "      bAlternateSetting:  %d\r\n", interface->bAlternateSetting);
  fusion_fprintf(file_handle, "      bNumEndpoints:      %d\r\n", interface->bNumEndpoints);
  fusion_fprintf(file_handle, "      bInterfaceClass:    0x%02X\r\n", interface->bInterfaceClass);
  fusion_fprintf(file_handle, "      bInterfaceSubClass: 0x%02X\r\n", interface->bInterfaceSubClass);
  fusion_fprintf(file_handle, "      bInterfaceProtocol: 0x%02X\r\n", interface->bInterfaceProtocol);
  fusion_fprintf(file_handle, "      iInterface:         %d\r\n", interface->iInterface);

  rotate();
}

void report_device_endpoint(const endpoint_descriptor *const endpoint, const int file_handle) __sdcccall(1) {
  fusion_fprintf(file_handle, "      Endpoint:\r\n");
  fusion_fprintf(file_handle, "        bLength:          %d\r\n", endpoint->bLength);
  fusion_fprintf(file_handle, "        bDescriptorType:  %d\r\n", endpoint->bDescriptorType);
  fusion_fprintf(file_handle, "        bEndpointAddress: 0x%02X\r\n", endpoint->bEndpointAddress);
  fusion_fprintf(file_handle, "        bmAttributes:     0x%02X\r\n", endpoint->bmAttributes);
  fusion_fprintf(file_handle, "        wMaxPacketSize:   %d\r\n", endpoint->wMaxPacketSize);
  fusion_fprintf(file_handle, "        bInterval:        %d\r\n", endpoint->bInterval);

  rotate();
}
