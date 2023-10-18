#include "class_printer.h"
#include "print.h"
#include "usb_state.h"
#include <delay.h>
#include <stdbool.h>
#include <stdint.h>

uint8_t prt_send_ch(const uint8_t ch) __z88dk_fastcall {
  _usb_state *const work_area = get_usb_work_area();

  usb_error result = prt_send_text(&work_area->printer_config, &ch, 1);

  return result == USB_ERR_OK ? 0 : -1;
}

// approx 100 chars per second (for 50hz)
// or 120 chars per second (for 60hz)
uint8_t USBPRT(const uint8_t ch) __z88dk_fastcall {
  _usb_state *const work_area = get_usb_work_area();

  // if (work_area->printer_time_throttle_flag == 0) {
  __asm ei __endasm;

  int16_t time = get_future_time(8);
  while (!is_time_past(time))
    ;
  // }

  // work_area->printer_time_throttle_flag += 1;
  // work_area->printer_time_throttle_flag &= 1;

  return prt_send_ch(ch);
}
