#ifndef __SERIAL_SIO2_H
#define __SERIAL_SIO2_H

#include <stdint.h>

extern uint8_t assigned_port_number;

#define SIO_BUFSZ 256

extern uint8_t  sio_data_count;
extern uint8_t  sio_buf[SIO_BUFSZ];
extern uint8_t *sio_buf_head;
extern uint8_t *sio_buf_tail;

/**
 * @brief probes and identify presence of SIO/2 chip
 *
 * @return uint8_t 0 -> none, 1 -> RC2014 mapping, 2 -> Alt mapping
 */
extern uint8_t sio_probe(void);

extern void sio2_chip_init(void);

#endif
