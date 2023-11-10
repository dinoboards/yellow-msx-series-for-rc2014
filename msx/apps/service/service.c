#include <msxdos.h>
#include <stdbool.h>
#include <stdio.h>

extern void transition(const void *const source, const uint16_t size) __sdcccall(1);

extern uint8_t transition_start[];
extern uint8_t transition_end[];

extern uint8_t temp_storage[];

uint8_t main(const int argc, const char *argv[]) {
  (void)argc;
  (void)argv;

  if (argc != 2) {
    printf("Usage: servioce=<service>.sys\r\n");
    return 1;
  }

  const char *serviceFileName = argv[1] + 1;

  memcpy((void *)0xC000, transition_start, transition_end - transition_start);

  const uint16_t error_handle = msxdosOpenFile(argv[1] + 1, IO_RDONLY);
  const uint8_t  handle       = error_handle & 0xFF;
  const uint8_t  error        = error_handle >> 8 & 0xFF;

  if (error) {
    char error_description[64];
    memset(error_description, 0, sizeof(error_description));
    msxdosExplainErrorCode(error, error_description);
    printf("%s (%02X): %s.\r\n", error_description, error, serviceFileName);
    return 1;
  }

  uint8_t *buffer      = temp_storage;
  uint16_t read_bytes  = 0;
  uint16_t total_bytes = 0;
  do {
    read_bytes = msxdosReadFile(handle, buffer, 1024);
    buffer += 1024;
    total_bytes += read_bytes;

  } while (read_bytes == 1024);

  msxdosCloseFile(handle);

  transition(temp_storage, total_bytes);

  return 0;
}
