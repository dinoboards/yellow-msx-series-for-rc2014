#include "fossil.h"
#include <msxbios/system_vars.h>
#include <msxdos.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <z80.h>

#define MSXDOS_TPA_BASE_ADDR __at(0x0006)

uint16_t MSXDOS_TPA_BASE_ADDR MSXDOS_TPA_BASE;

uint16_t __at(0x01aa) _restore_sp_onexit;

#define HIMSAV_ADDR    __at(0xf349)
#define DOSHIM_ADDR    __at(0xF34B)
#define NOTFIRST_ADDR  __at(0xF340)
#define LOAD_FLAG_ADDR __at(0x0037)

uint16_t HIMSAV_ADDR   HIMSAV;
uint16_t DOSHIM_ADDR   DOSHIM;
uint8_t NOTFIRST_ADDR  NOTFIRST;
uint8_t LOAD_FLAG_ADDR LOAD_FLAG;

#define MOVE_UP_LINE "\x1E"
#define ERASE_LINE   "\x1B\x6C\r"

// CONTAINS "RS" IF INSTALLED AND ACTIVE
#define FSMARK_ADDR __at(0xF3FC)

// FOSSIL JUMP TABLE ADDRESS
#define FSTABL_ADDR __at(0xF3FE)

char FSMARK_ADDR     FSMARK[2];
uint16_t FSTABL_ADDR FSTABL;

extern void transition(const void *const source, const uint16_t size) __sdcccall(1);

extern uint8_t transition_start[];
extern uint8_t transition_end[];
extern uint8_t temp_storage[];

extern uint8_t sys_start[];
extern uint8_t sys_end[];

extern uint16_t relocation_table_start[];
extern uint16_t relocation_table_end[];
#define relocation_table_size (((uint16_t)relocation_table_end - (uint16_t)relocation_table_start) / 2)

void reloadMSXDOS(void) {
  memcpy((void *)0xC000, transition_start, transition_end - transition_start);

  char boot_path[256];
  msxdosGetEnvironment("BOOT", boot_path, 255);
  char nextor_path[256];
  sprintf(nextor_path, "%s\\NEXTOR.SYS", boot_path);
  const uint16_t error_handle = msxdosOpenFile(nextor_path, IO_NO_WR);
  const uint8_t  handle       = error_handle & 0xFF;
  uint8_t        error        = error_handle >> 8 & 0xFF;

  if (error) {
    char error_description[64];
    memset(error_description, 0, sizeof(error_description));
    msxdosExplainErrorCode(error, error_description);
    printf("%s (%02X)\r\n", error_description, error);
    return;
  }

  uint8_t *buffer      = temp_storage;
  uint16_t read_bytes  = 0;
  uint16_t total_bytes = 0;
  do {
    read_bytes = msxdosReadFile(handle, buffer, 1024);
    buffer += 1024;
    total_bytes += read_bytes;

  } while (read_bytes == 1024);

  error = msxdosCloseFile(handle);

  sprintf((char *)0x0080, "\020FOSSIL 1");
  NOTFIRST = 0;
  transition(temp_storage, total_bytes);
  // we never return from here
}

#define relocated(x) ((void *)((uint16_t)(x) + (uint16_t)(HIMSAV + 1)))

uint8_t main(const int argc, const char *argv[]) {
  (void)argc;
  (void)argv;

  // printf("argc: %d\r\n", argc);
  // for (int i = 0; i < argc; i++) {
  //   printf("argv[%d]: %s\r\n", i, argv[i]);
  // }

  if (argc >= 2 && strncasecmp(argv[1], "=FOSSIL", 7) != 0) {
    printf(MOVE_UP_LINE ERASE_LINE "FOSSIL:          INSTALLED\r\n");

    char buffer[256];
    msxdosGetEnvironment("__SHELL", buffer, 255);
    msxdosSetEnvironment("SHELL", buffer);
    msxdosSetEnvironment("__SHELL", "");

    uint16_t fossil_driver_installed = HIMSAV + 1;
    memcpy((void *)fossil_driver_installed, sys_start, sys_end - sys_start);

    const uint16_t offset = fossil_driver_installed >> 8;
    for (uint16_t i = 0; i < relocation_table_size; i++) {
      uint8_t *p = (uint8_t *)(fossil_driver_installed + relocation_table_start[i]);
      *p += offset;
    }

    FSMARK[0] = 'S';
    FSMARK[1] = 'R';
    FSTABL    = fossil_driver_installed;

    return 0;
  }

  uint16_t driver_length = sys_end - sys_start;

  const uint16_t original_himsav = HIMSAV;
  const uint16_t new_location    = ((original_himsav - driver_length) & 0xFF00) - 1;

  HIMSAV = DOSHIM = new_location;

  printf("Reloading MSX-DOS... \r");

  char buffer[256];
  msxdosGetEnvironment("SHELL", buffer, 255);
  msxdosSetEnvironment("__SHELL", buffer);
  reloadMSXDOS();

  return 0;
}
