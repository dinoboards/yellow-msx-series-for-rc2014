#ifndef __USB_TYPES
#define __USB_TYPES

#include <stdlib.h>

#define GET_STATUS    0
#define CLEAR_FEATURE 1
#define SET_FEATURE   3

#define FEAT_PORT_POWER 8
#define FEAT_PORT_RESET 4

#define RT_HOST_TO_DEVICE 0b00000000
#define RT_DEVICE_TO_HOST 0b10000000
#define RT_STANDARD       0b00000000
#define RT_CLASS          0b00100000
#define RT_VENDOR         0b01000000
#define RT_DEVICE         0b00000000
#define RT_INTERFACE      0b00000001
#define RT_ENDPOINT       0b00000010
#define RT_OTHER          0b00000011

typedef struct _setup_packet {
  uint8_t  bmRequestType;
  uint8_t  bRequest;
  uint8_t  bValue[2];
  uint8_t  bIndex[2];
  uint16_t wLength;
} setup_packet;

typedef struct _device_descriptor {
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint16_t bcdUSB;
  uint8_t  bDeviceClass;
  uint8_t  bDeviceSubClass;
  uint8_t  bDeviceProtocol;
  uint8_t  bMaxPacketSize0;
  uint16_t idVendor;
  uint16_t idProduct;
  uint16_t bcdDevice;
  uint8_t  iManufacturer;
  uint8_t  iProduct;
  uint8_t  iSerialNumber;
  uint8_t  bNumConfigurations;
} device_descriptor;

typedef struct _config_descriptor {
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint16_t wTotalLength;
  uint8_t  bNumInterfaces;
  uint8_t  bConfigurationvalue;
  uint8_t  iConfiguration;
  uint8_t  bmAttributes;
  uint8_t  bMaxPower;
} config_descriptor;

typedef struct _interface_descriptor {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bInterfaceNumber;
  uint8_t bAlternateSetting;
  uint8_t bNumEndpoints;
  uint8_t bInterfaceClass;
  uint8_t bInterfaceSubClass;
  uint8_t bInterfaceProtocol;
  uint8_t iInterface;
} interface_descriptor, *p_interface_descriptor;

typedef struct _endpoint_descriptor {
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint8_t  bEndpointAddress;
  uint8_t  bmAttributes;
  uint16_t wMaxPacketSize;
  uint8_t  bInterval;
} endpoint_descriptor;

typedef struct {
  uint8_t  bDescLength;
  uint8_t  bDescriptorType;
  uint8_t  bNbrPorts;
  uint16_t wHubCharacteristics;
  uint8_t  bPwrOn2PwrGood;
  uint8_t  bHubContrCurrent;
  uint8_t  DeviceRemovable[1];
} hub_descriptor;

typedef struct {
  union {
    struct {
      uint8_t port_connection : 1;
      uint8_t port_enable : 1;
      uint8_t port_suspend : 1;
      uint8_t port_over_current : 1;
      uint8_t port_reset : 1;
      uint8_t reserved : 3;
      uint8_t port_power : 1;
      uint8_t port_low_speed : 1;
      uint8_t port_high_speed : 1;
      uint8_t port_test : 1;
      uint8_t port_indicator : 1;
    };

    uint16_t val;

  } wPortStatus;

  union {
    struct {
      uint8_t c_port_connection : 1;
      uint8_t c_port_enable : 1;
      uint8_t c_port_suspend : 1;
      uint8_t c_port_over_current : 1;
      uint8_t c_port_reset : 1;
    };

    uint16_t val;

  } wPortChange;

} hub_port_status;

#endif
