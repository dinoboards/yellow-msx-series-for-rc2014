#ifndef __SERIAL_H
#ifndef __SERIAL_H

// Common interface for accessing serial devices such as SIO/2 and USB FTDI

#define SERIAL_BITS_7 7
#define SERIAL_BITS_8 8

#define SERIAL_PARITY_NONE  0
#define SERIAL_PARITY_ODD   ((uint16_t)1 << 8)
#define SERIAL_PARITY_EVEN  ((uint16_t)2 << 8)
#define SERIAL_PARITY_MARK  ((uint16_t)3 << 8)
#define SERIAL_PARITY_SPACE ((uint16_t)4 << 8)

#define SERIAL_STOPBITS_1  0
#define SERIAL_STOPBITS_15 ((uint16_t)1 << 11)
#define SERIAL_STOPBITS_2  ((uint16_t)2 << 11)

#define SERIAL_BREAK_OFF 0
#define SERIAL_BREAK_ON  ((uint16_t)1 << 14)

#define SERIAL_FLOW_CTRL_DISABLE  0x0
#define SERIAL_FLOW_CTRL_RTS_CTS  (0x1 << 8)
#define SERIAL_FLOW_CTRL_DTR_DSR  (0x2 << 8)
#define SERIAL_FLOW_CTRL_XON_XOFF (0x4 << 8)

#define SIO_SET_DTR_MASK 0x1
#define SIO_SET_DTR_HIGH (1 | (SIO_SET_DTR_MASK << 8))
#define SIO_SET_DTR_LOW  (0 | (SIO_SET_DTR_MASK << 8))
#define SIO_SET_RTS_MASK 0x2
#define SIO_SET_RTS_HIGH (2 | (SIO_SET_RTS_MASK << 8))
#define SIO_SET_RTS_LOW  (0 | (SIO_SET_RTS_MASK << 8))

uint8_t serial_get_available_ports(uint8_t *available ports); // set number of install ports

uint8_t serial_get_driver_name(const uint8_t port_numnber, const char *driver_name); // get driver name (eg: 'sio2' or 'usbftdi')

uint8_t serial_reset(const uint8_t port_number);

uint8_t serial_set_protocol(const uint8_t port_number, const uint16_t protocol); // set protocol mask SERIAL_XXXX

uint8_t serial_set_baudrate(const uint8_t port_number, const int32_t baudrate);

uint8_t serial_set_flowcontrol(const uint8_t port_number, const uint16_t flowcontrol); // set flow control mask
                                                                                       // SERIAL_FLOW_CTRL_XXXX

uint8_t serial_set_dtr(const uint8_t port_number, const uint16_t dtr_rts); // set dtr/rts mask SIO_SET_DTR_HIGH/LOW

uint8_t serial_set_rts(const uint8_t port_number, const uint16_t dtr_rts); // set dtr/rts mask SIO_SET_RTS_HIGH/LOW

uint8_t serial_read(const uint8_t port_number, uint8_t *const buf, uint8_t *size); // return 0 if ok, otherwise error code

uint8_t
serial_send(const uint8_t port_number, const uint8_t *const buf, const uint8_t size); // return 0 if ok, otherwise error code

#endif
