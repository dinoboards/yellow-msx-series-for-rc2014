#ifndef __SYSLOAD_C
#define __SYSLOAD_C

#include "sys.h"
#include <stdint.h>

#define MAX_SYS 8

struct installed_sys_struct;
typedef struct installed_sys_struct installed_sys_t;

typedef uint16_t (*sys_init_fn)(installed_sys_t *my_header) __z88dk_fastcall;
typedef void (*sys_hook_fn)(void) __z88dk_fastcall;

#define REQUIRE_EXTBIO  1
#define REQUIRE_TIMI    2
#define REQUIRE_KEYI    4
#define INSTALL_SERVICE 0x8000

typedef struct installed_sys_struct {
  char    name[8]; /* name of the service */
  uint8_t reserved[8];
} installed_sys_t;

typedef struct {
  uint8_t     jump_op_code;
  sys_hook_fn address;
} jump_hook_t;

#define SYSTEM_JUMP_HOOKS   3
#define RESERVED_JUMP_HOOKS 7
#define FIRST_SYS_JUMP_HOOK (SYSTEM_JUMP_HOOKS + RESERVED_JUMP_HOOKS)
#define SYS_JUMP_HOOKS      (64 - SYSTEM_JUMP_HOOKS - RESERVED_JUMP_HOOKS)

typedef struct {
  jump_hook_t      first_extbio_handler; /* initialised to original_extbio_hook (redirected at 0x4000) */
  jump_hook_t      first_timi_handler;   /* initialised to original_timi_hook  (redirected at 0x4003) */
  jump_hook_t      first_keyi_handler;   /* initialised to original_timi_hook  (redirected at 0x4006) */
  jump_hook_t      reserved_jump_hooks[RESERVED_JUMP_HOOKS];
  jump_hook_t      jump_hooks[SYS_JUMP_HOOKS];
  uint8_t          original_extbio_hook[5];
  uint8_t          original_timi_hook[5];
  uint8_t          original_keyi_hook[5];
  uint8_t          count;        /* number of actual sys's installed */
  uint8_t         *next_address; /* address to install the next sys */
  installed_sys_t *sys[MAX_SYS];
} sys_segment_head_t;

extern sys_segment_head_t __at(0x4000) sys_segment_head;

typedef struct {
  jump_hook_t callmapi;
  uint8_t     slot;
  uint8_t     segment;
} call_mapi_jump_hook_t;

extern void install_extbio_hook(void);
extern void install_timi_hook(void);
extern void install_keyi_hook(void);

extern uint16_t init(installed_sys_t *my_header) __z88dk_fastcall;
extern uint8_t  allocate_a_segment_hook(void);
extern uint8_t  install_segment_hook(const uint16_t fn_address, call_mapi_jump_hook_t *const hook);

#endif
