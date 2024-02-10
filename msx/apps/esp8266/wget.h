#ifndef __WGET
#define __WGET

#include <stdint.h>

extern uint8_t wget(uint32_t *const retrieved_bytes) __z88dk_fastcall;
extern void    subCommandWGet(void);
extern uint8_t wget_retry(uint32_t *const retrieved_bytes, const uint8_t count);

#endif
