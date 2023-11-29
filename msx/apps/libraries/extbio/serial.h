#ifndef __RC2014_SERIAL_H
#define __RC2014_SERIAL_H

// Common interface for accessing serial devices such as SIO/2 and USB FTDI

#include <stdint.h>
#include <string.h>

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
#define SERIAL_FLOW_CTRL_RTS_CTS  (0x1)
#define SERIAL_FLOW_CTRL_DTR_DSR  (0x2)
#define SERIAL_FLOW_CTRL_XON_XOFF (0x4)

#define SERIAL_SET_DTR_MASK 0x1
#define SERIAL_SET_DTR_HIGH (1 | (SERIAL_SET_DTR_MASK << 8))
#define SERIAL_SET_DTR_LOW  (0 | (SERIAL_SET_DTR_MASK << 8))
#define SERIAL_SET_RTS_MASK 0x2
#define SERIAL_SET_RTS_HIGH (2 | (SERIAL_SET_RTS_MASK << 8))
#define SERIAL_SET_RTS_LOW  (0 | (SERIAL_SET_RTS_MASK << 8))

/**
 * @brief Retreive the number of installed serial port devices
 *
 * @param ports must be initialised to zero, will be incremented by each driver
 * @return uint8_t zero if ok, otherwise error code
 */
extern uint8_t serial_get_available_ports(uint8_t *const available_ports) __z88dk_fastcall;

/**
 * @brief Retrieve a name for the driver associated with the port number
 *
 * @param port_number
 * @param driver_name
 * @return uint8_t
 */
extern uint8_t serial_get_driver_name(const uint8_t port_number, char *const driver_name);

extern uint8_t serial_reset(const uint8_t port_number);

/**
 * @brief set serial bit protocol
 *
 * @param port_number
 * @param protocol combination of masks SERIAL_PARITY_XXX | SERIAL_STOPBITS_XXX | SERIAL_BITS_XXX | SERIAL_BREAK_XXX
 * @return uint8_t
 */
extern uint8_t serial_set_protocol(const uint8_t port_number, const uint16_t protocol);

extern uint8_t serial_set_baudrate(const uint8_t port_number, const int32_t baudrate);

extern uint8_t serial_set_flowcontrol(const uint8_t port_number, const uint16_t flowcontrol); // set flow control mask
                                                                                              // SERIAL_FLOW_CTRL_XXXX

extern uint8_t serial_set_dtr(const uint8_t port_number, const uint16_t dtr_rts); // set dtr/rts mask SIO_SET_DTR_HIGH/LOW

extern uint8_t serial_set_rts(const uint8_t port_number, const uint16_t dtr_rts); // set dtr/rts mask SIO_SET_RTS_HIGH/LOW

/**
 * @brief read data from the serial port into the buffer
 *
 * @param port_number
 * @param buf
 * @param size on entry, the size of the buffer, on exit the number of bytes actually received (max 64)
 * @return uint8_t 0 if ok, otherwise error code
 */
extern uint8_t serial_read(const uint8_t port_number, uint8_t *const buf, uint16_t *size);

/**
 * @brief read data from the serial port into the buffer
 * unlike serial_read, serial_demand_read will wait upto timeout_period_ms to
 * attempt to receive bytes as specified in size
 *
 * @param port_number
 * @param buf
 * @param size on entry the size of the buffer expected to be read, on exit the actual read byte count.
 * @param timeout_period
 * @return uint8_t
 */
extern uint8_t serial_demand_read(const uint8_t port_number, uint8_t *const buf, uint16_t *size, const uint16_t timeout_period_ms);

/**
 * @brief write the contents of the buffer to the serial port
 *
 * @param port_number
 * @param buf the contents to be written
 * @param size the size of the buffer to be written - max of 64
 * @return uint8_t 0 if ok, otherwise error code
 */
extern uint8_t serial_write(const uint8_t port_number, const uint8_t *const buf, const uint8_t size);

/**
 * @brief purge tx and rx buffers
 *
 * @param port_number
 * @return uint8_t
 */
extern uint8_t serial_purge_buffers(const uint8_t port_number) __z88dk_fastcall;

extern uint8_t serial_set_flowctrl(const uint8_t port_number, const int8_t flowctrl);

extern uint8_t serial_set_dtr_rts(const uint8_t port_number, const int16_t dtr_rts_flags);

extern uint8_t serial_get_diagnostics(const uint8_t port_number, void *p);

/**
 * @brief returns the number of bytes available in the RX buffer than can be read
 *
 * @param port_number
 * @return uint16_t
 */
extern uint16_t serial_get_rx_buffer_size(const uint8_t port_number) __z88dk_fastcall;

#endif
