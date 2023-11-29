#include "../sio2.h"
#include <string.h>
#include <z80.h>

uint8_t assigned_port_number;

extern uint8_t sio_data_count;
extern uint8_t sio_in(void);
extern void    sio_out(uint8_t data) __z88dk_fastcall;

uint32_t baudrate = 19200;

uint8_t _serial_get_driver_name(char *const driver_name) {
  strcpy(driver_name, "sio/2");
  return 0;
}

uint8_t _serial_set_baudrate(const int32_t _baudrate) {
  baudrate = _baudrate;

  switch (_baudrate) {
  case 4800L:
    sio_clock_divider = SIO_CLK_DIV_64;
    break;

  case 9600L:
    sio_clock_divider = SIO_CLK_DIV_32;
    break;

  case 19200L:
    sio_clock_divider = SIO_CLK_DIV_16;
    break;

  default:
    return 255;
  }

  sio2_configure_port();
  return 0;
}

uint8_t _serial_set_flowctrl(const int8_t flowctrl) {
  flowctrl;
  return 1;
}

uint8_t _serial_set_dtr_rts(const int16_t dtr_rts_flags) {
  dtr_rts_flags;
  return 1;
}

uint8_t _serial_set_protocol(const uint16_t protocol) {
  protocol;
  return 1;
}

uint8_t _serial_read(uint8_t *buf, uint16_t *size) {
  uint16_t remaining = *size;

  while (remaining > 0 && sio_data_count > 0) {
    *buf++ = sio_in();
    remaining--;
  }

  *size = *size - remaining;

  return 0;
}

uint8_t _serial_demand_read(uint8_t *buf, uint16_t *size, const uint16_t timeout_ms) {
  timeout_ms;
  uint16_t remaining = *size;

  while (remaining > 0 && sio_data_count > 0) {
    *buf++ = sio_in();
    remaining--;
  }

  *size = *size - remaining;

  return 0;
}

uint8_t _serial_write(const uint8_t *buf, uint8_t size) {
  while (size > 0) {
    sio_out(*buf++);
    size--;
  }
  return 0;
}

uint16_t _serial_get_rx_buffer_size(void) { return sio_data_count; }

uint8_t _serial_purge_buffers(void) {

  DI;
  sio_buf_head = sio_buf_tail = sio_buf;
  sio_data_count              = 0;
  EI;

  return 0;
}

uint8_t _serial_get_diagnostics(void *p) {
  sio_diagnostic_t *t = (sio_diagnostic_t *)p;

  t->sio_buf        = sio_buf;
  t->sio_buf_head   = sio_buf_head;
  t->sio_buf_tail   = sio_buf_tail;
  t->sio_data_count = sio_data_count;
  t->ch             = *sio_buf_tail;

  return 0;
}
