#include <system_vars.h>
#include <delay.h>
#include "print.h"
#include "work-area.h"

__sfr __at 0x84 CH376_DATA_PORT;
__sfr __at 0x85 CH376_COMMAND_PORT;

#define CH_CMD_RESET_ALL  0x05

inline void usb_reset() {
  CH376_COMMAND_PORT = CH_CMD_RESET_ALL;
  delay(6);
}

void usb_host_init() {
  void *p = get_work_area();
  xprintf("usb_host_init %p\r\n", p);

  usb_reset();
}
