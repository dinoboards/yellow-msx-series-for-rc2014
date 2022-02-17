#include <stdlib.h>
#include <string.h>

extern uint8_t WORKING_BUFFER[];

#define DRIVER_BANK_NUMBER 7;

// slot status
__sfr __at 0xA8 PSL_STAT;
__sfr __at 0xFE MEM_BANK_PAGE_2;

uint8_t          __at 0xC000 SEGS[16];
volatile uint8_t __at 0xFFFF SECONDARY_SLOT;

uint8_t __at 0x6000 BNKREG; // Bank selection register for nextor

uint8_t __at 0x5555 rom5555;
uint8_t __at 0x2AAA rom2AAA;

void erase_nextor_driver();
void reset_system();
void debug();

void main() {
  __asm__("LD	SP, 0xF000");

  erase_nextor_driver();

  reset_system();
}

inline void set_page_1_to_slot_33() {
  PSL_STAT = PSL_STAT | 0b00001100;
  SECONDARY_SLOT = (~SECONDARY_SLOT) | 0b00001100;
}

// initate a memory write for ASCII16 bank switching, to switch to driver bank
inline void set_ROM_driver_bank() { BNKREG = DRIVER_BANK_NUMBER; }

// switch in main rom to access 00000-07FFF of ROM
inline void set_page_0_and_1_for_ROM_programming() { PSL_STAT = PSL_STAT & 0b11110000; }

// switch page 1 to nextor rom (slot 3-3) - bank should have been preselected be on the selected bank
inline void set_page_1_to_nextor_slot() { PSL_STAT = PSL_STAT | 0b00001100; }

void erase_sector_prefix() {
  rom5555 = 0xAA;
  rom2AAA = 0x55;
  rom5555 = 0x80;
  rom5555 = 0xAA;
  rom2AAA = 0x55;
}

void program_byte_prefix() {
  rom5555 = 0xAA;
  rom2AAA = 0x55;
  rom5555 = 0xA0;
}

void wait_for_completion(volatile uint8_t *addr) __z88dk_fastcall {
  (void)addr;
  // clang-format off
  __asm

wait_for_erase:
  ld	a, (hl)
	cp	(hl)
	jr	nz, wait_for_erase

  __endasm;
  // clang-format on
}

void copy_4k_from_source_bank(uint8_t *source) __z88dk_fastcall {
  PSL_STAT = PSL_STAT | 0b00110000;

  // select RAM slot for page 2
  SECONDARY_SLOT = (~SECONDARY_SLOT) & 0b11001111 | 0b00100000;

  // select the memory bank
  MEM_BANK_PAGE_2 = SEGS[0];

  memcpy(WORKING_BUFFER, source, 0x1000);
}

void flash_4k(uint8_t *destination) __z88dk_fastcall {
  debug();

  uint8_t *source = WORKING_BUFFER;

  for (uint16_t index = 0x1000; index > 0; index--) {
    set_page_1_to_slot_33();
    set_ROM_driver_bank();
    set_page_0_and_1_for_ROM_programming();
    program_byte_prefix();
    set_page_1_to_nextor_slot();
    *destination = *source++; // writing to the bank, may cause it to also page swap
    wait_for_completion(destination++);
  }
}

void erase_sector(uint8_t *address) __z88dk_fastcall {
  set_page_1_to_slot_33();
  set_ROM_driver_bank();
  set_page_0_and_1_for_ROM_programming();
  erase_sector_prefix();
  set_page_1_to_nextor_slot();
  // write the erase code
  *address = 0x30; // not sure if this would cause a ascii 16 bank switch
  wait_for_completion(address);
}

void erase_nextor_driver() {
  copy_4k_from_source_bank((uint8_t *)0x8000);
  erase_sector((uint8_t *)0x4000);
  flash_4k((uint8_t *)0x4000);

  copy_4k_from_source_bank((uint8_t *)0x9000);
  erase_sector((uint8_t *)0x5000);
  flash_4k((uint8_t *)0x5000);

  copy_4k_from_source_bank((uint8_t *)0xA000);
  erase_sector((uint8_t *)0x6000);
  flash_4k((uint8_t *)0x6000);

  copy_4k_from_source_bank((uint8_t *)0xB000);
  erase_sector((uint8_t *)0x7000);
  flash_4k((uint8_t *)0x7000);
}

void reset_system() {
  // select slot 0 for page 0 and page 1
  PSL_STAT = PSL_STAT & 0b11110000;

  __asm__("JP 0");
}
