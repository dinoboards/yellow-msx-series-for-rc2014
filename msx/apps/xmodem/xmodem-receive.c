#include "arguments.h"
#include "crc16.h"
#include <extbio/serial-helpers.h>
#include <extbio/serial.h>
#include <libgen.h>
#include <msxbios.h>
#include <msxdos.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define SOH   0x01 // Start of Header
#define EOT   0x04 // End of Transmission
#define ACK   0x06 // Acknowledge
#define NAK   0x15 // Not Acknowledge
#define CAN   0x18 // Cancel
#define CTRLZ 0x1A // EOF

#define MAX_RETRIES 10
#define DATA_SIZE   128
#define PACKET_SIZE (DATA_SIZE + 5)

const unsigned char rotatingChar[4][5] = {STR_VERT_BAR CURSOR_MOVE_LEFT, STR_FOWARD_SLASH CURSOR_MOVE_LEFT,
                                          STR_DASH CURSOR_MOVE_LEFT, STR_BACK_SLASH CURSOR_MOVE_LEFT};

uint8_t  rotatingIndex   = 0;
uint32_t total_file_size = 0;
uint8_t  block[PACKET_SIZE];
uint8_t  block_number = 1;
uint8_t  retries      = 0;
uint8_t  fptr         = 0;
char     file_name_buffer[256];
char     p_temp_file_name[256];
uint8_t  error;
char    *pPath;
uint16_t error_handle;

static bool check_crc(void) {
  const unsigned char *buf = &block[3];
  uint8_t              counter;
  uint16_t             crc = 0;
  for (counter = 0; counter < DATA_SIZE; counter++)
    crc = (crc << 8) ^ crc16tab[((crc >> 8) ^ *buf++) & 0x00FF];

  const uint16_t tcrc = (*buf++ << 8) + *buf;

  return (crc == tcrc);
}

static uint8_t read_single_char(void) {
  uint16_t actual_size = 1;
  memset(block, 0, sizeof(block));
  const uint8_t r = serial_demand_read(port_number, block, &actual_size, 3000);

  return r == 0;
}

static uint8_t read_block(void) {
  uint16_t      actual_size = PACKET_SIZE - 1;
  const uint8_t r           = serial_demand_read(port_number, block + 1, &actual_size, 3000);
  return r == 0;
}

static void create_temp_file(void) {
  strcpy(file_name_buffer, p_file_name);
  pPath       = dirname(file_name_buffer);
  p_file_name = basename(p_file_name);
  sprintf(p_temp_file_name, "%sxmdwn.tmp", pPath, p_file_name);

  error_handle = msxdosCreateFile(p_temp_file_name, IO_NO_RD);
  error        = error_handle >> 8 & 0xFF;
  if (error) {
    char error_description[64];
    memset(error_description, 0, sizeof(error_description));
    msxdosExplainErrorCode(error, error_description);
    printf("%s (%02X): %s.\r\n", error_description, error, p_temp_file_name);
    exit(255);
    return;
  }

  fptr = error_handle & 0xFF;
}

#define WRITE_BUFFER_SIZE 2048
uint8_t  write_buffer[WRITE_BUFFER_SIZE];
uint16_t write_buffer_length = 0;

static void write_block_to_file(void) {
  memcpy(write_buffer + write_buffer_length, block + 3, DATA_SIZE);
  write_buffer_length += DATA_SIZE;
  if (write_buffer_length < WRITE_BUFFER_SIZE)
    return;

  error = msxdosWriteFile(fptr, write_buffer, WRITE_BUFFER_SIZE);
  if (error) {
    char error_description[64];
    memset(error_description, 0, sizeof(error_description));
    msxdosExplainErrorCode(error, error_description);
    printf("%s (%02X): %s.\r\n", error_description, error, p_temp_file_name);
    exit(255);
  }

  write_buffer_length = 0;
}

static void flush_write_buffer(void) {
  if (write_buffer_length == 0)
    return;

  error = msxdosWriteFile(fptr, write_buffer, write_buffer_length);
  if (error) {
    char error_description[64];
    memset(error_description, 0, sizeof(error_description));
    msxdosExplainErrorCode(error, error_description);
    printf("%s (%02X): %s.\r\n", error_description, error, p_temp_file_name);
    exit(255);
  }

  write_buffer_length = 0;
}

void xmodem_receive(void) {
  memset(write_buffer, 0, sizeof(write_buffer));

  create_temp_file();

  serial_purge_buffers(port_number);

  printf("Downloading '%s': ", p_file_name);

  serial_write_char('C');

  while (true) {
    if (!read_single_char()) {
    nak_and_retry:
      serial_purge_buffers(port_number);
      serial_write_char(NAK);
      retries++;

      if (retries > MAX_RETRIES) {
        printf(ERASE_LINE "'%s' failed to download. Too many errors\r\n", p_file_name);
        goto aborted;
      }

      continue;
    }

    if (block[0] == EOT) {
      serial_write_char(ACK);
      break;
    }

    if (block[0] != SOH) {
      goto nak_and_retry;
    }

    read_block();

    if (block[0] == CAN && block[1] == CAN && block[2] == CAN) {
      serial_write_char(ACK);
      goto aborted;
    }

    if (block[1] < block_number && block[2] == (uint8_t)~block[1]) {
      // ignore a previously received duplicated packet
      serial_write_char(ACK);
      retries = 0;
      continue;
    }

    if (block[1] != block_number) {
      goto nak_and_retry;
    }

    if (block[2] != (uint8_t)~block_number) {
      goto nak_and_retry;
    }

    if (!check_crc()) {
      goto nak_and_retry;
    }

    printf(rotatingChar[rotatingIndex]);
    rotatingIndex = (rotatingIndex + 1) & 3;

    write_block_to_file();
    total_file_size += DATA_SIZE;
    serial_write_char(ACK);
    block_number++;
    retries = 0;
  }

  flush_write_buffer();
  msxdosCloseFile(fptr);
  msxdosDeleteFile(p_file_name);
  msxdosRenameFile(p_file_name, p_temp_file_name);

  printf(ERASE_LINE "Downloaded '%s' (%ld bytes)\r\n", p_file_name, total_file_size);

  return;

aborted:
  msxdosCloseFile(fptr);
  msxdosDeleteFile(p_temp_file_name);
}
