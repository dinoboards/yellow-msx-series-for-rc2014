#include "flashloader.h"
#include <extbio.h>
#include <msxdos.h>
#include <stdio.h>
#include <system_vars.h>

memmap_extbio_info info_table;

void main() {
  printf("MSX-DOS Memory mapping interface testing....\r\n");

  extbio_get_dev_info_table(4, &info_table);

  printf("Slot ID %02X, Address %p\r\n", info_table.slot_id, info_table.jump_table);
  printf("Free Slot %d, Total Slots: %d\r\n", info_table.number_of_free_segments, info_table.number_of_segments);

  printf("Spike 5\r\n");
  flashLoader();

  // memmap_link(&info_table);

  // const uint8_t page_0_slot_number = memmap_get_page_0();

  // printf("Page 0 slot number is: %d\r\n", page_0_slot_number);

  // uint8_t *p = info_table.jump_table;

  // printf("%02X %02X %02X\r\n", p[0], p[1], p[2]);

  // memmap_link(info_table.jump_table);

  // uint8_t slot_number = memmap_get_page_0();
  // printf("P0: %04X\r\n", slot_number);

  // slot_number = memmap_get_page_1();
  // printf("P1: %04X\r\n", slot_number);

  // slot_number = memmap_get_page_2();
  // printf("P2: %04X\r\n", slot_number);

  // slot_number = memmap_get_page_3();
  // printf("P3: %04X\r\n", slot_number);

  // uint8_t allocated_segment;
  // uint8_t allocated_slot;
  // uint8_t success = memmap_allocate_segment(0, 0x8B, &allocated_segment, &allocated_slot);

  // printf("Success: %d, seg: %02X, slot: %02X\r\n", success, allocated_segment, allocated_slot);

  // extbio_get_dev_info_table(4, &info_table);

  // printf("Slot ID %02X, Address %p\r\n", info_table.slot_id, info_table.jump_table);
  // printf("Free Slot %d, Total Slots: %d\r\n", info_table.number_of_free_segments, info_table.number_of_segments);

  // memmap_put_page_2(allocated_segment);
  // slot_number = memmap_get_page_2();
  // printf("P2: %04X\r\n", slot_number);
}
