#ifndef __XMODEM_SERIAL
#define __XMODEM_SERIAL

#include <extbio/serial.h>
#include <stdbool.h>

#define VDP_FREQUENCY 50

extern uint8_t serial_read_char_h(uint8_t *const c) __sdcccall(1);
extern uint8_t serial_write_char_h(const uint8_t c);
extern uint8_t serial_write_string_h(const uint8_t *str) __sdcccall(1);
extern uint8_t serial_write_bytes_h(const uint8_t *str, const uint8_t len) __sdcccall(1);
extern uint8_t find_serial_driver(const char *driver_name) __sdcccall(1);

#endif
