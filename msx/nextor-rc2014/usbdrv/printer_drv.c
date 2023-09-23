#include "class_printer.h"
#include "print.h"
#include "work-area.h"
#include <delay.h>
#include <stdbool.h>
#include <stdint.h>

uint8_t prt_send_ch(const uint8_t ch) __z88dk_fastcall {
  _usb_state *const work_area = get_usb_work_area();
  char              buffer[2] = {ch, 0};

  usb_error result = prt_send_text(&work_area->printer_config, buffer);

  return result == USB_ERR_OK ? 0 : -1;
}

// approx 100 chars per second (for 50hz)
// or 120 chars per second (for 60hz)
uint8_t USBPRT(const uint8_t ch) __z88dk_fastcall {
  _usb_state *const work_area = get_usb_work_area();

  if (work_area->printer_time_throttle_flag == 0) {
    __asm ei __endasm;

    int16_t time = get_future_time(1);
    while (!is_time_past(time))
      ;
  }

  work_area->printer_time_throttle_flag += 1;
  work_area->printer_time_throttle_flag &= 3;

  return prt_send_ch(ch);
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
