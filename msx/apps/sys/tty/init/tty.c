#include <stdio.h>
#include <sysload/sysload.h>

#define relocated(x)  ((const char *)((uint16_t)(&x) + (uint16_t)(my_header)))
#define relocated2(x) ((void *)((uint16_t)(x) + (uint16_t)(my_header)))
#define relocated3(x) ((jump_hook_t *)((uint16_t)(&x) + (uint16_t)(my_header)))

extern jump_hook_t original_hchpu;
extern void        chput(void);

uint8_t install_chput_hook(const uint16_t chput) {
  if (install_segment_hook(chput, &H_CHPU) == 0)
    return 255;

  return 0;
}

uint16_t init(installed_sys_t *my_header) __z88dk_fastcall {
  printf("my_header: %p\r\n", my_header);

  *relocated3(original_hchpu) = H_CHPU;
  install_chput_hook((uint16_t)relocated2(chput));

  return INSTALL_SERVICE | REQUIRE_EXTBIO | REQUIRE_TIMI;
}
