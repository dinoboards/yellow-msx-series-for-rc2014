#include <stdlib.h>
#include <string.h>

extern uint8_t WORKING_BUFFER[];

// slot status
__sfr __at 0xA8 PSL_STAT;
__sfr __at 0xFE MEM_BANK_PAGE_2;

uint8_t          __at 0xC000 SEGS[16];
volatile uint8_t __at 0xFFFF SECONDARY_SLOT;

uint8_t __at 0x5555 rom5555;
uint8_t __at 0x2AAA rom2AAA;

void erase_extended_rom();
void erase_sector_prefix();
void wait_for_completion(volatile uint8_t *addr) __z88dk_fastcall;
void copy_4k_from_source_bank();
void flash_4k();
void reset_system();

void main() {
  __asm__("LD	SP, 0xF000");

  erase_extended_rom();

  copy_4k_from_source_bank();

  flash_4k();

  reset_system();
}

void erase_sector_prefix() {
  rom5555 = 0xAA;
  rom2AAA = 0x55;
  rom5555 = 0x80;
  rom5555 = 0xAA;
  rom2AAA = 0x55;
}

void erase_extended_rom() {
  // select slot 0 for page 0 and page 1
  // and select slot 3 for page 2
  PSL_STAT = PSL_STAT & 0b11110000 | 0b00110000;

  // select sub slot 3 for page 2
  SECONDARY_SLOT = (~SECONDARY_SLOT) | 0b00110000;

  volatile uint8_t *addr = (uint8_t *)0x8000;

  for (uint8_t c = 4; c > 0; c--) {
    erase_sector_prefix();
    *addr = 0x30;
    wait_for_completion(addr);
    addr += 0x100;
  }
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

void copy_4k_from_source_bank() {
  // select TAM slot for page 2
  SECONDARY_SLOT = (~SECONDARY_SLOT) & 0b11001111 | 0b00100000;

  // select the memory bank
  MEM_BANK_PAGE_2 = SEGS[0];

  memcpy(WORKING_BUFFER, (uint8_t *)0x8000, 0x1000);
}

void program_byte_prefix() {
  rom5555 = 0xAA;
  rom2AAA = 0x55;
  rom5555 = 0xA0;
}

void flash_4k() {
  // select slot 0 for page 0 and page 1
  // and select slot 3 for page 2
  PSL_STAT = PSL_STAT & 0b11110000 | 0b00110000;

  // select sub slot 3 for page 2
  SECONDARY_SLOT = (~SECONDARY_SLOT) | 0b00110000;

  uint8_t *source = WORKING_BUFFER;
  uint8_t *destination = (uint8_t *)0x8000;

  for (uint16_t index = 0x1000; index > 0; index--) {
    program_byte_prefix();
    *destination = *source++;
    wait_for_completion(destination++);
  }
}

void reset_system() {
  // select slot 0 for page 0 and page 1
  PSL_STAT = PSL_STAT & 0b11110000;

  __asm__("JP 0");
}
