#ifndef __SERIAL_SIO2_H
#define __SERIAL_SIO2_H

#include <stdint.h>

extern uint8_t assigned_port_number;

#define SIO_BUFSZ 256

extern uint8_t  sio_data_count;
extern uint8_t  sio_buf[SIO_BUFSZ];
extern uint8_t *sio_buf_head;
extern uint8_t *sio_buf_tail;
extern uint8_t  sio_clock_divider;

#define SIO_CLK_DIV_64 0b11000000
#define SIO_CLK_DIV_32 0b10000000
#define SIO_CLK_DIV_16 0b01000000

/**
 * @brief probes and identify presence of SIO/2 chip
 *
 * @return uint8_t 0 -> none, 1 -> RC2014 mapping, 2 -> Alt mapping
 */
extern uint8_t sio_probe(void);

extern void sio2_configure_port(void);

typedef struct {
  uint8_t *sio_buf;
  uint8_t *sio_buf_head;
  uint8_t *sio_buf_tail;
  uint8_t  sio_data_count;
  uint8_t  ch;
} sio_diagnostic_t;

#endif
