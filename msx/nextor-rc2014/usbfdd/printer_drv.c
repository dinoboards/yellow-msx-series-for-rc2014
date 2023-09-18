#include <stdint.h>
#include "class-printer.h"
#include "work-area.h"
#include <stdbool.h>
#include "print.h"

uint8_t USBPRT(const uint8_t ch) __z88dk_fastcall {
   _usb_state *const work_area = get_usb_work_area();

  char buffer[5] = {ch, 0};

  usb_error result = prt_send_text(&work_area->printer, buffer);

  return result;
}

#define H_LPTO_ADDR  __at 0xFFB6
#define RST_30 0xF7

typedef struct {
  uint8_t rst;
  uint8_t slot;
  uint16_t addr;
} jump_hook;


jump_hook H_LPTO_ADDR H_LPTO;

void install_printer(const _usb_state *const work_area) __z88dk_fastcall {
  const bool hasPrinter = work_area->printer.address != 0;

  if(!hasPrinter)
    return;

  H_LPTO.rst = RST_30;
  H_LPTO.slot = 0x8F; //slot 3-3
  H_LPTO.addr = (uint16_t)0x785C; //DRV_DIRECT4;
}
