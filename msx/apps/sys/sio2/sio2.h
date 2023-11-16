#ifndef __SERIAL_SIO2_H
#define __SERIAL_SIO2_H

#include <stdint.h>

extern uint8_t assigned_port_number;

/**
 * @brief probes and identify presence of SIO/2 chip
 *
 * @return uint8_t 0 -> none, 1 -> RC2014 mapping, 2 -> Alt mapping
 */
extern uint8_t sio_probe(void);

#endif
