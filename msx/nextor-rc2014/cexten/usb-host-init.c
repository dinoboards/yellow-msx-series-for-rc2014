#include <system_vars.h>
#include <delay.h>
#include "print.h"
#include "work-area.h"
#include <stdbool.h>

__sfr __at 0x84 CH376_DATA_PORT;
__sfr __at 0x85 CH376_COMMAND_PORT;

#define CH_CMD_GET_IC_VER   0x01
#define CH_CMD_RESET_ALL    0x05
#define CH_CMD_CHECK_EXIST  0x06
#define CH_CMD_SET_USB_MODE 0x15
#define CH_CMD_SET_RETRY    0x0B


#define CH_MODE_HOST_RESET  7
#define CH_MODE_HOST        6

// return codes
#define CH_ST_RET_SUCCESS         0x51
#define CH_ST_RET_ABORT           0x5F

inline void ch376_reset() {
  CH376_COMMAND_PORT = CH_CMD_RESET_ALL;
  delay(60);
}

/* =============================================================================

  Check if the USB host controller hardware is operational
  
  Returns:
    1 is operation, 0 if not

============================================================================= */
inline uint8_t ch376_test() {
  CH376_COMMAND_PORT = CH_CMD_CHECK_EXIST;
  CH376_DATA_PORT = (uint8_t)~0x34;
  if (CH376_DATA_PORT != 0x34)
    return false;

  CH376_COMMAND_PORT = CH_CMD_CHECK_EXIST;
  CH376_DATA_PORT = (uint8_t)~0x89;
  return (CH376_DATA_PORT == 0x89);
}

/* =============================================================================

  Retrieve the CH376 chip version

  Returns:
    The chip version
============================================================================= */
inline uint8_t ch376_get_firmware_version() {
  CH376_COMMAND_PORT = CH_CMD_GET_IC_VER;
  return CH376_DATA_PORT & 0x1f;
}

/* =============================================================================

  Set the CH376 USB MODE

  Returns:
    0 -> OK, 1 -> ERROR
============================================================================= */
uint8_t ch376_set_usb_mode(uint8_t mode) __z88dk_fastcall {
  CH376_COMMAND_PORT = CH_CMD_SET_USB_MODE;
  CH376_DATA_PORT = mode;

  uint8_t count = 255;
  while( CH376_DATA_PORT != CH_ST_RET_SUCCESS && --count != 0)
    ;

  return count != 0;
}

void hw_configure_nak_retry() {
  CH376_COMMAND_PORT = CH_CMD_SET_RETRY;
  CH376_DATA_PORT = 0x25;
  CH376_DATA_PORT = 0x8F;   // Retry NAKs indefinitely (default)
}

uint8_t usb_host_bus_reset() {
  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60/4);

  ch376_set_usb_mode(CH_MODE_HOST_RESET);
  delay(60/2);

  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60/4);

  hw_configure_nak_retry();

  return true;
}

uint8_t usb_host_init() {
  work_area *p = get_work_area();
  xprintf("usb_host_init %p\r\n", p);

  ch376_reset();

  if (!ch376_test()) {
    xprintf("CH376:           NOT PRESENT\r\n");
    return false;
  }

  p->ch376.present = true;
  const uint8_t ver = ch376_get_firmware_version();
  xprintf("CH376:           PRESENT (VER %d)\r\n", ver);

  if (!usb_host_bus_reset())
    return false;


  return true;
}
