#include <string.h>
#include <system_vars.h>
#include <usb/class_printer.h>

// overload the use of the system var LOWLIM (used during tape load/save) to
// optmised interrupt handle when print buffer is empty.

device_config_printer _printer_config;

void drv_timi_printer(void) {
  if (!(LOWLIM & 0x80))
    return;

  if (_printer_config.buffer_length == 0) {
    _printer_config.buffer_wait = 0;
    LOWLIM &= 0x7F;
    return;
  }

  if (_printer_config.buffer_length != PRINTER_BUFFER_SIZE && _printer_config.buffer_wait != 5) {
    _printer_config.buffer_wait++;

    return;
  }

  device_config_printer device;
  memcpy(&device, &_printer_config, sizeof(device_config_printer));

  usb_data_out_transfer(device.buffer, device.buffer_length, device.address, &device.endpoints[0]);
  memcpy(&_printer_config, &device, sizeof(device_config_printer));

  _printer_config.buffer_length = 0;
  _printer_config.buffer_wait   = 1;
  LOWLIM &= 0x7F;
}

uint8_t lpt_output(const uint8_t ch) __sdcccall(1) {

  while (_printer_config.buffer_length >= PRINTER_BUFFER_SIZE) {
    __asm__("EI");
    __asm__("HALT");
  }

  __asm__("DI");
  _printer_config.buffer[_printer_config.buffer_length++] = ch;
  LOWLIM |= 0x80;
  __asm__("EI");

  return 0;
}
