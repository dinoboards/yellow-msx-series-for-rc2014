#include "class_printer.h"
#include "print.h"
#include "work-area.h"
#include <delay.h>
#include <stdbool.h>
#include <stdint.h>
#include <system_vars.h>
#include <transfers.h>

// overload the use of the system var LOWLIM (used during tape load/save) to
// optmised interrupt handle when print buffer is empty.

void drv_timi(void) {
  if (!(LOWLIM & 0x80))
    return;

  device_config_printer *const printer_config = &get_usb_work_area()->printer_config;

  if (printer_config->buffer_length == 0) {
    printer_config->buffer_wait = 0;
    LOWLIM &= 0x7F;
    return;
  }

  if (printer_config->buffer_length != PRINTER_BUFFER_SIZE && printer_config->buffer_wait != 5) {
    printer_config->buffer_wait++;

    return;
  }

  usb_data_out_transfer(printer_config->buffer, printer_config->buffer_length, printer_config->address,
                        &printer_config->endpoints[0]);
  printer_config->buffer_length = 0;
  printer_config->buffer_wait   = 1;
  LOWLIM &= 0x7F;
}

uint8_t USBPRT(const uint8_t ch) __z88dk_fastcall {
  device_config_printer *const printer_config = &get_usb_work_area()->printer_config;

  while (printer_config->buffer_length >= PRINTER_BUFFER_SIZE) {
    __asm__("EI");
    __asm__("HALT");
  }

  __asm__("DI");
  printer_config->buffer[printer_config->buffer_length++] = ch;
  LOWLIM |= 0x80;
  __asm__("EI");

  return 0;
}

#define H_LPTS_ADDR __at 0xFFBB
#define H_LPTO_ADDR __at 0xFFB6
#define RST_30      0xF7

#define POP_IX_1 0xDD
#define POP_IX_2 0xE1
#define OR       0xF6
#define RET      0xC9

typedef struct {
  uint8_t  rst;
  uint8_t  slot;
  uint16_t addr;
} jump_hook;

typedef struct {
  uint8_t bytes[5];
} jump_hook_disable;

jump_hook H_LPTO_ADDR         H_LPTO;
jump_hook_disable H_LPTS_ADDR H_LPTS;

void install_printer(const _usb_state *const work_area) __z88dk_fastcall {
  const bool hasPrinter = work_area->printer_config.address != 0;

  if (!hasPrinter)
    return;

  H_LPTO.rst  = RST_30;
  H_LPTO.slot = 0x8F;             // slot 3-3
  H_LPTO.addr = (uint16_t)0x785C; // DRV_DIRECT4;

  H_LPTS.bytes[0] = POP_IX_1;
  H_LPTS.bytes[1] = POP_IX_2;
  H_LPTS.bytes[2] = OR;
  H_LPTS.bytes[3] = 0xFF;
  H_LPTS.bytes[4] = RET;
}
