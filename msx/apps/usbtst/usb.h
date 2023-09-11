#ifndef __USB_TYPES
#define __USB_TYPES

#include <stdlib.h>

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

/*
wHubCharacteristics:
  D1...D0: Logical Power Switching Mode
    00: Ganged power switching (all ports power at once)
    01: Individual port power switching
    1X: Reserved. Used only on 1.0 compliant hubs that implement no power switching

  D2: Identifies a Compound Device
    0: Hub is not part of a compound device.
    1: Hub is part of a compound device.

  D4...D3: Logical Power Switching Mode
    00: Global Over-current Protection. The hub reports over-current as a summation
        of all ports’ current draw, without a breakdown of individual port
        over-current status.
    01: Individual Port Over-current Protection. The hub reports over-current on a
        per-port basis. Each port has an over-current status.
    1X: No Over-current Protection. This option is  allowed only for bus-powered
*/

typedef struct {
  uint8_t  bDescLength;
  uint8_t  bDescriptorType;     /* HUB Descriptor Type 0x29 */
  uint8_t  bNbrPorts;           /* Number of ports */
  uint16_t wHubCharacteristics; /* Bitmap	Hub Characteristics (see above) */
  uint8_t  bPwrOn2PwrGood;      /* Time (*2 ms) from port power on to power good */
  uint8_t  bHubContrCurrent;    /* Maximum current used by hub controller (mA).*/
  uint8_t  DeviceRemovable[1];  /* bits indicating deviceRemovable and portPwrCtrlMask */
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
