#ifndef _USB_CLASS_FTDI_H
#define _USB_CLASS_FTDI_H

#include <stdint.h>
#include <usb/ch376.h>
#include <usb/dev_transfers.h>

/** Parity mode for ftdi_set_line_property() */
enum ftdi_parity_type { NONE = 0, ODD = 1, EVEN = 2, MARK = 3, SPACE = 4 };
/** Number of stop bits for ftdi_set_line_property() */
enum ftdi_stopbits_type { STOP_BIT_1 = 0, STOP_BIT_15 = 1, STOP_BIT_2 = 2 };
/** Number of bits for ftdi_set_line_property() */
enum ftdi_bits_type { BITS_7 = 7, BITS_8 = 8 };
/** Break type for ftdi_set_line_property2() */
enum ftdi_break_type { BREAK_OFF = 0, BREAK_ON = 1 };

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

#define SIO_RESET_SIO      0
#define SIO_RESET_PURGE_RX 1
#define SIO_RESET_PURGE_TX 2


#define C_CLK 48000000 /* the 232R's clock rate */

extern usb_error ftdi_set_baudrate(device_config_ftdi *const ftdi, int32_t *const baudrate) __sdcccall(1);
extern usb_error ftdi_set_line_property2(device_config_ftdi *const ftdi,
                                         enum ftdi_bits_type       bits,
                                         enum ftdi_stopbits_type   sbit,
                                         enum ftdi_parity_type     parity,
                                         enum ftdi_break_type      break_type);
extern usb_error ftdi_read_data(device_config_ftdi *const ftdi, uint8_t *buf, uint8_t *const size);
extern usb_error ftdi_write_data(device_config_ftdi *const ftdi, const uint8_t *const buf, const uint16_t size);
extern usb_error ftdi_usb_purge_tx_buffer(device_config_ftdi *const ftdi) __sdcccall(1);
extern usb_error ftdi_usb_purge_rx_buffer(device_config_ftdi *const ftdi) __sdcccall(1);

#endif
