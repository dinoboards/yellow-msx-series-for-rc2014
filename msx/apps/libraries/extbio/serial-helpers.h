#ifndef __XMODEM_SERIAL
#define __XMODEM_SERIAL

#include <extbio/serial.h>
#include <stdbool.h>

#define VDP_FREQUENCY 50

extern uint8_t serial_read_char(uint8_t *const c) __sdcccall(1);
extern uint8_t serial_write_char(const uint8_t c) __sdcccall(1);
extern uint8_t serial_write_string(const uint8_t *str) __sdcccall(1);
extern uint8_t serial_write_bytes(const uint8_t *str, const uint8_t len) __sdcccall(1);

#endif
