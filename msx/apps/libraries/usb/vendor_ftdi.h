#ifndef _USB_CLASS_FTDI_H
#define _USB_CLASS_FTDI_H

#include <stdint.h>
#include <usb/ch376.h>
#include <usb/dev_transfers.h>

/* Definitions for flow control */
#define SIO_RESET         0 /* Reset the port */
#define SIO_MODEM_CTRL    1 /* Set the modem control register */
#define SIO_SET_FLOW_CTRL 2 /* Set flow control register */
#define SIO_SET_BAUD_RATE 3 /* Set baud rate */
#define SIO_SET_DATA      4 /* Set the data characteristics of the port */

#define FTDI_DEVICE_OUT_REQTYPE (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT)
#define FTDI_DEVICE_IN_REQTYPE  (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN)

/* Requests */
#define SIO_RESET_REQUEST             SIO_RESET
#define SIO_SET_BAUDRATE_REQUEST      SIO_SET_BAUD_RATE
#define SIO_SET_DATA_REQUEST          SIO_SET_DATA
#define SIO_SET_FLOW_CTRL_REQUEST     SIO_SET_FLOW_CTRL
#define SIO_SET_MODEM_CTRL_REQUEST    SIO_MODEM_CTRL
#define SIO_POLL_MODEM_STATUS_REQUEST 0x05
#define SIO_SET_EVENT_CHAR_REQUEST    0x06
#define SIO_SET_ERROR_CHAR_REQUEST    0x07
#define SIO_SET_LATENCY_TIMER_REQUEST 0x09
#define SIO_GET_LATENCY_TIMER_REQUEST 0x0A
#define SIO_SET_BITMODE_REQUEST       0x0B
#define SIO_READ_PINS_REQUEST         0x0C
#define SIO_READ_EEPROM_REQUEST       0x90
#define SIO_WRITE_EEPROM_REQUEST      0x91
#define SIO_ERASE_EEPROM_REQUEST      0x92

extern usb_error ftdi_set_baudrate(device_config_ftdi *const ftdi, int32_t baudrate) __sdcccall(1);

#endif
