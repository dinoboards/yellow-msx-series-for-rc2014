#include "sysload.h"
#include <extbio.h>
#include <io.h>
#include <msxdos.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

extern installed_sys_t sys;
extern uint8_t         sys_start[];
extern uint8_t         sys_end[];

extern uint16_t relocation_table_start[];
extern uint16_t relocation_table_end[];

sys_segment_head_t __at(0x4000) sys_segment_head;

extern uint16_t extbio_next;
extern uint16_t extbio;
extern uint16_t timi_next;
extern uint16_t timi;
extern uint16_t keyi_next;
extern uint16_t keyi;
#define relocated(x) ((uint16_t *)((uint16_t)(&x) + (uint16_t)(sys_segment_head.sys[index])))

#define relocation_table_size (((uint16_t)relocation_table_end - (uint16_t)relocation_table_start) / 2)

#define to_next_page(x) (((uint16_t)x & 0xFF00) + 0x0100)

uint8_t *address;
char     buffer[128];
uint8_t  allocated_segment;
uint8_t  current_segment;
uint16_t file_handle;
uint16_t bytes_to_read;
uint16_t read_bytes;
uint16_t relocation_index;
uint16_t call_mapi;

memmap_extbio_info info_table;

/**
 * @brief allocate a system segment
 * initialise a state structure at the start of the segment
 * in prep for loading syss
 * set ENV 'SYSSEG' to segment number
 */
void create(void) {
  uint8_t allocated_slot = 0xFF;

  uint8_t error = memmap_allocate_segment(MEMMAP_SYSTEM, MEMAP_MAIN_SLOT, &allocated_segment, &allocated_slot);

  if (error) {
    printf("Failed to allocate segment\r\n");
    return;
  }

  uint8_t current_segment = memmap_get_page_1();
  memmap_put_page_1(allocated_segment);

  // Clear the segment
  memset((void *)0x4000, 0, 0x4000);

  sys_segment_head.next_address = (uint8_t *)to_next_page(0x4000 + sizeof(sys_segment_head_t));

  install_extbio_hook();
  install_timi_hook();
  install_keyi_hook();

  memmap_put_page_1(current_segment);

  char envBuffer[16];
  sprintf(envBuffer, "%d", allocated_segment);
  msxdosSetEnvironment("SYSSEG", envBuffer);
}

uint8_t get_sys_segment(void) {
  memset(buffer, 0, 128);

  uint16_t error = msxdosGetEnvironment("SYSSEG", buffer, 128);

  if (buffer[0] == 0) {
    create();
    error = msxdosGetEnvironment("SYSSEG", buffer, 128);
  }

  // extract the segment number
  uint16_t allocated_segment = 0;
  sscanf(buffer, "%d", &allocated_segment);

  return allocated_segment;
}

uint8_t check_segment(void) {
  allocated_segment = get_sys_segment();
  current_segment   = memmap_get_page_1();
  memmap_put_page_1(allocated_segment);

  if ((sys_segment_head.count + 1) >= MAX_SYS) {
    printf("Too many SYSs\r\n");
    return -1;
  }

  return 0;
}

uint8_t check_already_installed(void) {
  for (uint8_t i = 0; i < sys_segment_head.count; i++) {
    if (strncmp(sys_segment_head.sys[i]->name, sys.name, 8) == 0) {
      printf("SYS %s already installed\r\n", sys.name);
      return -1;
    }
  }

  return 0;
}

uint16_t main(const int argc, const char *argv[]) {
  argc;
  argv;
  uint16_t result = 1;

  const uint16_t length = sys_end - sys_start;

  extbio_get_dev_info_table(4, &info_table);
  memmap_link(info_table.jump_table);

  const uint16_t reduced_mapper_table = 0;
  const uint8_t  number               = 0;

  const jump_table_entry *hl = unapi_get_ram_helper(&reduced_mapper_table, &number);
  call_mapi                  = hl[2].address;

  if (check_segment())
    goto finally_memmap;

  if (check_already_installed())
    goto finally_memmap;

  uint8_t index = sys_segment_head.count;

  sys_segment_head.sys[index] = (installed_sys_t *)sys_segment_head.next_address;

  memcpy(sys_segment_head.next_address, sys_start, sys_end - sys_start);

  const uint16_t offset = ((uint16_t)sys_segment_head.next_address) >> 8;
  for (uint16_t i = 0; i < relocation_table_size; i++) {
    uint8_t *p = (uint8_t *)(sys_segment_head.next_address + relocation_table_start[i]);
    *p += offset;
  }

  result = init(sys_segment_head.sys[index]);

  if (result)
    goto finally_memmap;

  sys_segment_head.count++;
  sys_segment_head.next_address = (uint8_t *)to_next_page(sys_segment_head.next_address + sys_end - sys_start);
  const uint8_t flags           = sys_segment_head.sys[index]->flags;

  if (flags & REQUIRE_EXTBIO) {
    *relocated(extbio_next)                       = (uint16_t)sys_segment_head.first_extbio_handler.address;
    sys_segment_head.first_extbio_handler.address = (sys_hook_fn)relocated(extbio);
  }

  if (flags & REQUIRE_TIMI) {
    *relocated(timi_next)                       = (uint16_t)sys_segment_head.first_timi_handler.address;
    sys_segment_head.first_timi_handler.address = (sys_hook_fn)relocated(timi);
  }

  if (flags & REQUIRE_KEYI) {
    *relocated(keyi_next)                       = (uint16_t)sys_segment_head.first_keyi_handler.address;
    sys_segment_head.first_keyi_handler.address = (sys_hook_fn)relocated(keyi);
  }

finally_memmap:
  memmap_put_page_1(current_segment);

  return result;
}

uint8_t allocate_a_segment_hook(void) {
  for (uint8_t i = FIRST_SYS_JUMP_HOOK; i < SYS_JUMP_HOOKS; i++)
    if (sys_segment_head.jump_hooks[i].address == NULL)
      return i;

  return 0;
}

uint8_t install_segment_hook(const uint16_t fn_address, call_mapi_jump_hook_t *const hook) {
  const uint8_t slot_index = allocate_a_segment_hook();

  if (slot_index == 0)
    return 0;

  hook->callmapi.jump_op_code = 0xCD; // call
  hook->callmapi.address      = (sys_hook_fn)call_mapi;
  hook->slot                  = slot_index;
  hook->segment               = allocated_segment;

  sys_segment_head.jump_hooks[slot_index - FIRST_SYS_JUMP_HOOK].jump_op_code = 0xC3; // jump
  sys_segment_head.jump_hooks[slot_index - FIRST_SYS_JUMP_HOOK].address      = (sys_hook_fn)fn_address;

  return slot_index;
}
