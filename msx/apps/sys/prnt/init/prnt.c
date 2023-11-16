#include "../prnt.h"
#include <extbio_rc2014.h>
#include <msx_fusion.h>
#include <stdio.h>
#include <string.h>
#include <sysload/sys.h>
#include <sysload/sysload.h>
#include <system_vars.h>
#include <usb/class_printer.h>
#include <usb/find_device.h>

typedef struct {
  uint8_t address;
  uint8_t config_index;
  uint8_t interface_index;
} device_config_interface;

extern device_config_printer _printer_config;
device_config_printer       *printer_config;

extern uint8_t lpt_output(const uint8_t ch) __sdcccall(1);
extern void    drv_timi_printer(void);

bool is_printer(const device_descriptor *const device_descriptor, const interface_descriptor *const interface) __sdcccall(1) {
  device_descriptor;
  return (interface->bInterfaceClass == 7);
}

#define H_LPTS_ADDR __at 0xFFBB
#define H_LPTO_ADDR __at 0xFFB6
#define RST_30      0xF7

#define POP_IX_1 0xDD
#define POP_IX_2 0xE1
#define OR       0xF6
#define RET      0xC9

typedef struct {
  uint8_t bytes[5];
} jump_hook_disable;

call_mapi_jump_hook_t H_LPTO_ADDR H_LPTO;
jump_hook_disable H_LPTS_ADDR     H_LPTS;

uint8_t install_printer(const uint16_t lpt_output) {
  if (install_segment_hook(lpt_output, &H_LPTO) == 0)
    return 255;

  H_LPTS.bytes[0] = POP_IX_1;
  H_LPTS.bytes[1] = POP_IX_2;
  H_LPTS.bytes[2] = OR;
  H_LPTS.bytes[3] = 0xFF;
  H_LPTS.bytes[4] = RET;

  return 0;
}

#define relocated(x)  ((void *)((uint16_t)(&x) + (uint16_t)(my_header)))
#define relocated2(x) ((void *)((uint16_t)(x) + (uint16_t)(my_header)))

uint16_t init(installed_sys_t *my_header) __z88dk_fastcall {

  printer_config = ((device_config_printer *)relocated(_printer_config));

  usb_error result = find_device(is_printer, (device_config *)printer_config);

  if (result == 0) {
    if (install_printer((uint16_t)relocated2(lpt_output))) {
      printf("Failed to install usb printer driver due to lack of jump hooks\r\n");
      return 255;
    }

    printf("USB/PRINTER:     INSTALLED\r\n");
    return 0;
  }

  if (result != 255) {
    printf("USB error %02x\r\n", result);
    return result;
  }

  printf("USB/PRINTER:     NOT PRESENT\r\n");

  return 255;
}
