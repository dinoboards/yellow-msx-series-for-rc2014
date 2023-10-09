#ifndef __MEMMAPPER
#define __MEMMAPPER

#include <stdlib.h>

extern uint8_t memmap_allocate_segment(uint8_t for_user, uint8_t mapper_slot, uint8_t *allocated_segment, uint8_t *allocated_slot);
extern uint8_t memmap_get_page_0(void);
extern void    memmap_put_page_0(uint8_t segment) __z88dk_fastcall;
extern uint8_t memmap_get_page_1(void);
extern void    memmap_put_page_1(uint8_t segment) __z88dk_fastcall;
extern uint8_t memmap_get_page_2(void);
extern void    memmap_put_page_2(uint8_t segment) __z88dk_fastcall;
extern uint8_t memmap_get_page_3(void);
extern void    memmap_put_page_3(uint8_t segment) __z88dk_fastcall;

#endif
