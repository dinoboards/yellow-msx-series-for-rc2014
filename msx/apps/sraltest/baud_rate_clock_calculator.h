#ifndef __BAUD_RATE_CLOCK_CALCULATOR_H
#define __BAUD_RATE_CLOCK_CALCULATOR_H

#include <stdint.h>

#define C_CLK 48000000 /* the 232R's clock rate */

int32_t ftdi_convert_baudrate(int32_t baudrate, uint16_t *value, uint16_t *index);

#endif
