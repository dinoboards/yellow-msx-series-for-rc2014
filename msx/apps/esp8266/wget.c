#include "wget.h"
#include "arguments.h"
#include "esp8266.h"
#include "print.h"
#include <delay.h>
#include <extbio/serial-helpers.h>
#include <extbio/serial.h>
#include <msx/libgen.h>
#include <msxbios/msxbios.h>
#include <msxdos.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <system_vars.h>
#include <xmodem/receive.h>

static bool     started;
static uint32_t totalFileSize;
static uint32_t actual_retrieved_bytes;
static uint16_t error_handle;
static uint8_t  error;
static char    *pPath;
static char    *pFileName;
static char     fileNameBuffer[256];
static char     pTempFileName[256];
char            serial_write_temp_buffer[256];

void subCommandWGet(void) {
  printf("Attempting to retrieve file ");
  printf(pFilePathName);
  printf(" from:\r\n");
  printf(pWgetUrl);
  printf("\r\n");

  printf("Downloading '%s' ", pFilePathName);

  wget(&actual_retrieved_bytes);

  printf(ERASE_LINE "Downloaded '%s' (%ld bytes)\r\n", pFilePathName, actual_retrieved_bytes);
}

uint8_t wget(uint32_t *const retrieved_bytes) __z88dk_fastcall {
  started       = false;
  totalFileSize = 0;

  serial_purge_buffers(port_number);
  serial_write_string_h("\r\nat+wget");
  serial_write_string_h(pWgetUrl);
  serial_write_string_h("\r\n");

  if (serial_read_line() || strncmp(responseStr, "OK", 3) != 0) {
    resetModem();

    serial_purge_buffers(port_number);
    serial_write_string_h("\r\nat+wget");
    serial_write_string_h(pWgetUrl);
    serial_write_string_h("\r\n");

    if (serial_read_line() || strncmp(responseStr, "OK", 3) != 0) {
      printf("\r\nError requesting file:\r\n%s\r\n", responseStr);
      serial_purge_buffers(port_number);
      abortWithError(NULL);
      return 1;
    }
  }

  return xmodem_receive(pFilePathName, retrieved_bytes, true);
}

uint8_t wget_retry(uint32_t *const retrieved_bytes, const uint8_t count) {
  uint8_t result = 0;
  for (uint8_t i = 0; i < count; i++) {
    result = wget(retrieved_bytes);
    if (result == 0)
      break;
  }
  return result;
}
