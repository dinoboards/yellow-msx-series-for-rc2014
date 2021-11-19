#include "flashloader.h"
#include "memmapper.h"
#include <extbio.h>
#include <msxdos.h>
#include <stdio.h>
#include <system_vars.h>

memmap_extbio_info info_table;

const char pFileName[] = "TEST.ROM";

void main() {
  printf("SPIKE loading file into mapped memory ....\r\n");

  extbio_get_dev_info_table(4, &info_table);
  memmap_link(info_table.jump_table);
  printf("Free Slot %d, Total Slots: %d\r\n", info_table.number_of_free_segments, info_table.number_of_segments);

  FILE *fptr = NULL;
  fptr = fopen(pFileName, "rb");

  fseek(fptr, 0, SEEK_END); // seek to end of file
  long size = ftell(fptr);  // get current file pointer
  fseek(fptr, 0, SEEK_SET); // seek back to beginning of file

  printf("File size is %ld\r\n", size);

  if (size != 524288L) {
    printf("Rom image size is invalid - expected 512k\r\n");
    fclose(fptr);
    exit(1);
  }

  uint8_t currentSegment = memmap_get_page_2();

  uint8_t allocated_segment;
  uint8_t allocated_slot;

  for (int i = 0; i < 32; i++) {
    uint8_t success = memmap_allocate_segment(0, 0x0, &allocated_segment, &allocated_slot);

    memmap_put_page_2(allocated_segment);

    size_t length = fread((void *)0x8000, 1, 0x4000, fptr);

    printf("Read block %d %d bytes into segment %d\r\n", i, length, allocated_segment);
  }

  fclose(fptr);

  printf("File close\r\n");

  memmap_put_page_2(currentSegment);

  extbio_get_dev_info_table(4, &info_table);

  printf("Free Slot %d, Total Slots: %d\r\n", info_table.number_of_free_segments, info_table.number_of_segments);
}

// void main() {
//   printf("MSX-DOS Memory mapping interface testing....\r\n");

//   extbio_get_dev_info_table(4, &info_table);

//   printf("Slot ID %02X, Address %p\r\n", info_table.slot_id, info_table.jump_table);
//   printf("Free Slot %d, Total Slots: %d\r\n", info_table.number_of_free_segments, info_table.number_of_segments);

//   // printf("Spike 5\r\n");
//   // flashLoader();

//   uint8_t *p = info_table.jump_table;

//   printf("%02X %02X %02X\r\n", p[0], p[1], p[2]);

//   memmap_link(info_table.jump_table);

//   uint8_t slot_number = memmap_get_page_0();
//   printf("P0: %04X\r\n", slot_number);

//   slot_number = memmap_get_page_1();
//   printf("P1: %04X\r\n", slot_number);

//   slot_number = memmap_get_page_2();
//   printf("P2: %04X\r\n", slot_number);

//   slot_number = memmap_get_page_3();
//   printf("P3: %04X\r\n", slot_number);

//   uint8_t allocated_segment;
//   uint8_t allocated_slot;
//   uint8_t success = memmap_allocate_segment(0, 0x0, &allocated_segment, &allocated_slot);

//   printf("Success: %d, seg: %02X, slot: %02X\r\n", success, allocated_segment, allocated_slot);

//   extbio_get_dev_info_table(4, &info_table);

//   printf("Slot ID %02X, Address %p\r\n", info_table.slot_id, info_table.jump_table);
//   printf("Free Slot %d, Total Slots: %d\r\n", info_table.number_of_free_segments, info_table.number_of_segments);

//   memmap_put_page_2(allocated_segment);
//   slot_number = memmap_get_page_2();
//   printf("P2: %04X\r\n", slot_number);
// }
