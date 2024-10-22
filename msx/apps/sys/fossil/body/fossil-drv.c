#include <extbio/serial.h>
#include <z80.h>

#define MAX_CHANNELS 2

#define BUFFER_SIZE 256

typedef struct {
  uint8_t *next_byte;
  uint8_t *last_byte;
  uint8_t  incoming[BUFFER_SIZE];
} channel_buffer_t;

channel_buffer_t channels[MAX_CHANNELS] = {{channels[0].incoming, channels[0].incoming},
                                           {channels[1].incoming, channels[1].incoming}};

uint8_t           current_port_number = 1;
channel_buffer_t *current_channel     = &channels[0];

/**
 * @brief initialises RS232
 *
 */
uint16_t fossil_init(void) {
  // serial_reset(1);
  return serial_purge_buffers(1);
}

uint32_t baudrates[] = {75, 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};

/**
 * @brief Set the baud rates
 *
 * @param baud high byte TX baud rate, Low Byte RX baud rate
                                ; 0  = 75	   1  = 300	    2  = 600
                                ; 3  = 1200	 4  = 2400    5  = 4800
                                ; 6  = 9600	 7  = 19200	  8  = 38400
                                ; 9  = 57600 11 = 115200 12 = 230400
        ; 13 = 460800 14 = 921600

 * @return uint16_t return the actual selected rates
 */
uint16_t set_baud(const uint16_t baud) __z88dk_fastcall {
  const uint8_t txbaud = baud >> 8;
  const uint8_t rxbaud = baud & 0xFF;

  uint8_t lowestbaud = (txbaud < rxbaud) ? txbaud : rxbaud;

  if (lowestbaud < 1)
    lowestbaud = 1;

  if (lowestbaud > 14)
    lowestbaud = 14;

  serial_set_baudrate(current_port_number, baudrates[lowestbaud]);
  // serial_set_flowctrl(current_port_number, SERIAL_FLOW_CTRL_RTS_CTS);

  return lowestbaud | lowestbaud << 8;
}

/**
 * @brief Set the protocol object
 *
 * @param protocol
 *    0-7 reserved - set to 0
 *    8-9 data bits
                          00 5 bits or less
                          01 6 bits
                          10 7 bits
                          11 8 bits

                        10-11 stop bits
                          01 1 stopbit
                          10 1.5 stopbits
                          11 2 stopbits

      12-13 parity
                          00 none
                          01 even
                          11 odd

      14-15 0
 * @return uint16_t
 */
uint16_t set_protocol(const uint16_t protocol) __z88dk_fastcall {
  uint16_t serial_bits;

  switch ((protocol >> 8) & 0x3) {
  case 2:
    serial_bits = SERIAL_BITS_7;
    break;
  case 3:
    serial_bits = SERIAL_BITS_8;
    break;
  default:
    return -1; // not supported
  }

  const uint16_t serial_stop = (((protocol >> 10) & 0x3) - 1) << 11;

  uint16_t serial_parity;
  switch ((protocol >> 12) & 0x3) {
  case 0:
    serial_parity = SERIAL_PARITY_NONE;
    break;
  case 1:
    serial_parity = SERIAL_PARITY_EVEN;
    break;
  case 3:
    serial_parity = SERIAL_PARITY_ODD;
    break;
  default:
    return -1; // not supported
  }

  return serial_set_protocol(current_port_number, serial_bits | serial_stop | serial_parity | SERIAL_BREAK_OFF);
}

/**
 * @brief set serial channel
 *
 * @param channel (H) high byte channel number (0 or 1), (L) low byte ignored
 */
uint16_t set_channel(const uint16_t _channel) __z88dk_fastcall {
  uint8_t desired_channel = (_channel >> 8) + 1;

  uint8_t ports  = 0;
  uint8_t result = serial_get_available_ports(&ports);

  if (result || desired_channel > ports)
    return -1;

  current_port_number = desired_channel;
  current_channel     = &channels[current_port_number - 1];

  return 0;
}

uint16_t size = BUFFER_SIZE;

uint8_t read_into_channel(void) {
  size                 = BUFFER_SIZE;
  const uint8_t result = serial_read(current_port_number, current_channel->incoming, &size);
  if (result)
    return result;
  current_channel->next_byte = current_channel->incoming;
  current_channel->last_byte = current_channel->incoming + size;
  return 0;
}

/**
 * @brief retrieve a byte from the serial port
 *
 * @return uint8_t
 */
uint8_t rs_in(void) __sdcccall(1) {
  while (current_channel->next_byte == current_channel->last_byte)
    read_into_channel();

  return *current_channel->next_byte++;
}

/**
 * @brief send byte to serial port
 *
 * @param byte
 */
void rs_out(const uint8_t byte) __sdcccall(1) { serial_write(current_port_number, &byte, 1); }

/**
 * @brief retrieve incoming buffer status
 * A=0 No data in buffer, A!=0 data in buffer
 * The F register is set according the result
 *
 * @return uint8_t 0 if no data, nonzero if one of more bytes available
 */
uint8_t rs_in_stat(void) __sdcccall(1) {
  if (current_channel->next_byte != current_channel->last_byte)
    return 1;

  if (read_into_channel())
    return 0;

  return current_channel->next_byte != current_channel->last_byte;
}

/**
 * @brief ; retrieving count of characters in the receive buffer
 *
 * @return uint16_t number of characters in receive buffer
 */
uint16_t chars_in_buf(void) {
  if (current_channel->next_byte != current_channel->last_byte)
    return current_channel->last_byte - current_channel->next_byte;

  if (read_into_channel())
    return 0;

  return current_channel->last_byte - current_channel->next_byte;
}
