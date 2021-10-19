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

  // memmap_link(&info_table);

  // const uint8_t page_0_slot_number = memmap_get_page_0();

  // printf("Page 0 slot number is: %d\r\n", page_0_slot_number);
}
