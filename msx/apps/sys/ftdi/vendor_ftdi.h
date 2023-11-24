#ifndef _USB_CLASS_FTDI_H
#define _USB_CLASS_FTDI_H

#include <stdbool.h>
#include <stdint.h>
#include <usb/ch376.h>
#include <usb/dev_transfers.h>

typedef struct {
  uint8_t status[2];
  uint8_t data[62];
} ftdi_packet;

typedef struct {
  COMMON_DEVICE_CONFIG
  endpoint_param endpoints[2];        // Bulk in and out
  uint8_t        bitbang_enabled : 1; /* 0: normal mode 1: any of the bitbang modes enabled */
  uint32_t       baudrate;

  ftdi_packet hold;
  uint8_t     hold_size;

} device_config_ftdi;

typedef struct {
  uint32_t requested_baud_rate;
  uint32_t actual_baud_rate;
  uint16_t value;
  uint16_t index;
} baud_rate_clock_divisor;

#define FTDI_BITS_7 7
#define FTDI_BITS_8 8

#define FTDI_PARITY_NONE  0
#define FTDI_PARITY_ODD   ((uint16_t)1 << 8)
#define FTDI_PARITY_EVEN  ((uint16_t)2 << 8)
#define FTDI_PARITY_MARK  ((uint16_t)3 << 8)
#define FTDI_PARITY_SPACE ((uint16_t)4 << 8)

#define FTDI_STOPBITS_1  0
#define FTDI_STOPBITS_15 ((uint16_t)1 << 11)
#define FTDI_STOPBITS_2  ((uint16_t)2 << 11)

#define FTDI_BREAK_OFF 0
#define FTDI_BREAK_ON  ((uint16_t)1 << 14)

#define SIO_DISABLE_FLOW_CTRL 0x0
#define SIO_RTS_CTS_HS        (0x1)
#define SIO_DTR_DSR_HS        (0x2)
#define SIO_XON_XOFF_HS       (0x4)

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

#define SIO_SET_DTR_MASK 0x1
#define SIO_SET_DTR_HIGH (1 | (SIO_SET_DTR_MASK << 8))
#define SIO_SET_DTR_LOW  (0 | (SIO_SET_DTR_MASK << 8))
#define SIO_SET_RTS_MASK 0x2
#define SIO_SET_RTS_HIGH (2 | (SIO_SET_RTS_MASK << 8))
#define SIO_SET_RTS_LOW  (0 | (SIO_SET_RTS_MASK << 8))

extern usb_error ftdi_set_clks(device_config_ftdi *const ftdi, const uint16_t value, const uint16_t index);
extern usb_error ftdi_set_baudrate(device_config_ftdi *const ftdi, const int32_t baudrate);
extern usb_error ftdi_set_line_property2(device_config_ftdi *const ftdi, const uint16_t protocol_bits);
extern usb_error ftdi_read_data(device_config_ftdi *const ftdi, uint8_t *const buf, uint16_t *size) __sdcccall(1);
extern usb_error
ftdi_demand_read_data(device_config_ftdi *const ftdi, uint8_t *buf, uint16_t *const size, const uint16_t timeout_ms) __sdcccall(1);
#define ftdi_write_data(ftdi, buf, size) usbdev_bulk_out_transfer((device_config *)(ftdi), buf, size)
extern usb_error ftdi_purge_tx_buffer(device_config_ftdi *const ftdi);
extern usb_error ftdi_purge_rx_buffer(device_config_ftdi *const ftdi);

extern usb_error ftdi_set_flowctrl(device_config_ftdi *const ftdi, uint8_t flowctrl);
extern usb_error ftdi_set_dtr_rts(device_config_ftdi *const ftdi, const uint16_t dtr_rts_flags);

extern usb_error ftdi_poll_modem_status(device_config_ftdi *const ftdi, uint16_t *const status);

extern uint8_t ftdi_get_rx_buffer_size(device_config_ftdi *const ftdi) __sdcccall(1);

#endif
