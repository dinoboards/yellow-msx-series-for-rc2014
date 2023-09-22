#ifndef __HW
#define __HW

#include "ch376.h"
#include <stdlib.h>

typedef struct {
  usb_device_type type;
  uint8_t         max_packet_size;
  uint8_t         value;
  uint8_t         interface_number;
  uint8_t         tag;
  uint8_t         address;
  endpoint_param  endpoints[3]; // bulk in/out and interrupt
} device_config;

#define GET_STATUS      0
#define CLEAR_FEATURE   1
#define SET_FEATURE     3
#define GET_DESCRIPTOR  6
#define SET_DESCRIPTOR  7
#define CLEAR_TT_BUFFER 8
#define RESET_TT        9
#define GET_TT_STATE    10
#define CSTOP_TT        11

#define FEAT_PORT_POWER                    8
#define FEAT_PORT_RESET                    4
#define HUB_FEATURE_PORT_CONNECTION_CHANGE 16
#define FEAT_PORT_ENABLE_CHANGE            17
#define FEAT_PORT_RESET_CHANGE             20

// HUB_FEATURE_PORT_CONNECTION          = 0,
// HUB_FEATURE_PORT_ENABLE              = 1,
// HUB_FEATURE_PORT_SUSPEND             = 2,
// HUB_FEATURE_PORT_OVER_CURRENT        = 3,
// HUB_FEATURE_PORT_RESET               = 4,

// HUB_FEATURE_PORT_POWER               = 8,
// HUB_FEATURE_PORT_LOW_SPEED           = 9,

// HUB_FEATURE_PORT_CONNECTION_CHANGE   = 16,
// HUB_FEATURE_PORT_ENABLE_CHANGE       = 17,
// HUB_FEATURE_PORT_SUSPEND_CHANGE      = 18,
// HUB_FEATURE_PORT_OVER_CURRENT_CHANGE = 19,
// HUB_FEATURE_PORT_RESET_CHANGE        = 20,
// HUB_FEATURE_PORT_TEST                = 21,
// HUB_FEATURE_PORT_INDICATOR           = 22

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

extern usb_error hw_control_transfer(const setup_packet *const cmd_packet,
                                     void *const               buffer,
                                     const uint8_t             device_address,
                                     const uint8_t             max_packet_size);

extern usb_error hw_get_description(device_descriptor *const buffer);

extern usb_error hw_get_config_descriptor(config_descriptor *const buffer,
                                          const uint8_t            config_index,
                                          const uint8_t            buffer_size,
                                          const uint8_t            device_address,
                                          const uint8_t            max_packet_size);

extern usb_error
hw_data_in_transfer(uint8_t *buffer, const uint16_t buffer_size, const uint8_t device_address, endpoint_param *const endpoint);

extern usb_error
hw_data_out_transfer(const uint8_t *buffer, uint16_t buffer_size, const uint8_t device_address, endpoint_param *const endpoint);

extern usb_error hw_set_address(const uint8_t device_address) __z88dk_fastcall;

extern usb_error hw_set_configuration(const device_config *const config) __z88dk_fastcall;

#endif
