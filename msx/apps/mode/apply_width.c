#include "apply_width.h"
#include "argument_width.h"
#include <msxbios/msxbios.h>
#include <msxbios/system_vars.h>

uint8_t apply_width_settings(void) {
  if (col_width >= 33) {
    LINL40 = col_width;
    msxbiosInitxt();
  } else {
    LINL32 = col_width;
    msxbiosInit32();
  }

  return 0;
}
