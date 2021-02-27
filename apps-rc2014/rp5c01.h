#ifndef __RP5C01
#define __RP5C01

#include <stdbool.h>
#include <stdlib.h>

extern uint8_t  rp5c01Detect();
extern uint16_t rp5c01GetByte(uint8_t index);
extern uint8_t  _rp5c01SetByte(uint16_t r);

#define rp5c01SetByte(index, data) _rp5c01SetByte((((uint16_t)(index)) << 8) + (data))

#endif
