#ifndef _HOOKS_H
#define _HOOKS_H

#include <stdint.h>
#include <z80.h>

typedef void (*sys_hook_fn)(void) __z88dk_fastcall;

typedef union {
  struct {
    uint8_t bytes[5];
  };
  struct {
    uint8_t   jump_op_code; // JMP or CALL
    jump_fn_t address;
    uint8_t   unused[2];
  };
  struct {
    z80_jump_t callmapi;
    uint8_t    slot;
    uint8_t    segment;
  };
} jump_hook_t;

#define H_TIMI_ADDR __at(0xFD9F)
#define H_CHPU_ADDR __at(0xFDA4)
#define H_LPTS_ADDR __at(0xFFBB)
#define H_LPTO_ADDR __at(0xFFB6)

// called at start of interrupt subroutine, when it is clear that
// the interrupt is from the VDP
extern jump_hook_t H_TIMI_ADDR H_TIMI;

// called at start of CHPUT(00A2)
extern jump_hook_t H_CHPU_ADDR H_CHPU;

// called at start of LPTSTT(00A8)
extern jump_hook_t H_LPTS_ADDR H_LPTS;

// called at start of LPTOUT(00A5)
extern jump_hook_t H_LPTO_ADDR H_LPTO;

#endif
