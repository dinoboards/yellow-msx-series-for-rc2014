#include "hooks.h"

// called at start of LPTSTT(00A8)
jump_hook_t H_LPTS_ADDR H_LPTS;

// called at start of CHPUT(00A2)
jump_hook_t H_CHPU_ADDR H_CHPU;

// called at start of interrupt subroutine, when it is clear that
// the interrupt is from the VDP
jump_hook_t H_TIMI_ADDR H_TIMI;

// called at start of LPTOUT(00A5)
jump_hook_t H_LPTO_ADDR H_LPTO;
