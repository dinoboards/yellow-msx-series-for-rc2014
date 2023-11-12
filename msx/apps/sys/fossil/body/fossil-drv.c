#include <extbio/serial.h>

#define MAX_CHANNELS 2

typedef struct {
  uint8_t *next_byte;
  uint8_t *last_byte;
  uint8_t  incoming[64];
} channel_buffer;

channel_buffer channels[MAX_CHANNELS] = {{channels[0].incoming, channels[0].incoming},
                                         {channels[1].incoming, channels[1].incoming}};

uint8_t current_channel = 1;

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

  serial_set_baudrate(current_channel, baudrates[lowestbaud]);

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

  return serial_set_protocol(current_channel, serial_bits | serial_stop | serial_parity | SERIAL_BREAK_OFF);
}

/**
 * @brief set serial channel
 *
 * @param channel (H) high byte chanel number, (L) low byte ignored
 */
uint16_t set_channel(const uint16_t _channel) __z88dk_fastcall {
  uint8_t desired_channel = (_channel >> 8) + 1;

  uint8_t ports  = 0;
  uint8_t result = serial_get_available_ports(&ports);

  if (result || desired_channel > ports)
    return -1;

  current_channel = desired_channel;

  return 0;
}

/**
 * @brief retrieve a byte from the serial port
 *
 * @return uint8_t
 */
uint8_t rs_in(void) __sdcccall(1) {
  channel_buffer *const channel_buffer = &channels[current_channel];
  if (channel_buffer->next_byte == NULL) {
    uint8_t size = 64;
    serial_read(1, channel_buffer->incoming, &size);
    channel_buffer->next_byte = channel_buffer->incoming;
    channel_buffer->last_byte = channel_buffer->incoming + size;
  }

  if (channel_buffer->next_byte == channel_buffer->last_byte) {
    // TODO: this doesnt seem right - should we be blocking??
    return 0;
  }

  return *channel_buffer->next_byte++;
}

/**
 * @brief send byte to serial port
 *
 * @param byte
 */
void rs_out(const uint8_t byte) __sdcccall(1) { serial_write(current_channel, &byte, 1); }

/**
 * @brief retrieve incoming buffer status
 * A=0 No data in buffer, A!=0 data in buffer
                                ; The F register is set according the result
 *
 * @return uint8_t 0 if no data, nonzero if one of more bytes available
 */
uint8_t rs_in_stat(void) __sdcccall(1) {
  channel_buffer *const channel = &channels[current_channel];

  if (channel->next_byte != channel->last_byte)
    return 1;

  uint8_t size  = 64;
  uint8_t error = serial_read(1, channel->incoming, &size);
  if (error != 0)
    return 0;

  channel->next_byte = channel->incoming;
  channel->last_byte = channel->incoming + size;

  return channel->next_byte != channel->last_byte;
}

/**
 * @brief ; retrieving count of characters in the receive buffer
 *
 * @return uint16_t number of characters in receive buffer
 */
uint16_t chars_in_buf(void) {
  channel_buffer *const channel = &channels[current_channel];

  if (channel->next_byte != channel->last_byte)
    return channel->last_byte - channel->next_byte;

  uint8_t size  = 64;
  uint8_t error = serial_read(1, channel->incoming, &size);
  if (error != 0)
    return 0;

  channel->next_byte = channel->incoming;
  channel->last_byte = channel->incoming + size;

  return size;
}

void drv_timi(void) {}
