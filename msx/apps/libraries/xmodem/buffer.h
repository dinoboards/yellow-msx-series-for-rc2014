#ifndef __XMODEM_BUFFER_H
#define __XMODEM_BUFFER_H

#include <stdint.h>

#define DATA_SIZE 128

#define WRITE_BUFFER_SIZE 2048
extern uint8_t write_buffer[WRITE_BUFFER_SIZE];

#endif
