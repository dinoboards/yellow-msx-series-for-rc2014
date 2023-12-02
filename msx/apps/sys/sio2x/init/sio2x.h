#ifndef __SIO2X_H
#define __SIO2X_H

#include <stdint.h>

/**
 * @brief probes and identify presence of SIO/2 chip
 *
 * @return uint8_t 0 -> none, 1 -> RC2014 mapping, 2 -> Alt mapping
 */
extern uint8_t sio_probe(void);

extern void sio2_chip_init(void);
extern void sio2_chip_spike(void);

#endif
