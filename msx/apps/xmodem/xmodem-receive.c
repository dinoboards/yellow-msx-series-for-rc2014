#include "arguments.h"
#include "crc16.h"
#include <extbio/serial-helpers.h>
#include <extbio/serial.h>
#include <libgen.h>
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
  printf("Requesting %d bytes\r\n", actual_size);
  memset(block, 0, sizeof(block));
  const uint8_t r = serial_demand_read(port_number, block, &actual_size, 3000);
  printf("Received %d bytes\r\n", actual_size);
  for (uint8_t i = 0; i < actual_size; i++) {
    printf("%02X ", block[i]);
  }

  return r == 0;
}

static uint8_t read_block(void) {
  uint16_t actual_size = PACKET_SIZE - 1;
  printf("Requesting %d bytes\r\n", actual_size);
  const uint8_t r = serial_demand_read(port_number, block + 1, &actual_size, 3000);
  printf("Received %d bytes\r\n", actual_size);
  for (uint8_t i = 0; i < actual_size; i++) {
    printf("%02X ", block[i]);
  }

  return r == 0;
}

static void create_temp_file(void) {
  strcpy(file_name_buffer, p_file_name);
  pPath = dirname(file_name_buffer);
  printf("%s, %s\r\n", file_name_buffer, pPath);
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

static void write_block_to_file(void) {
  error = msxdosWriteFile(fptr, block + 3, DATA_SIZE);
  if (error) {
    char error_description[64];
    memset(error_description, 0, sizeof(error_description));
    msxdosExplainErrorCode(error, error_description);
    printf("%s (%02X): %s.\r\n", error_description, error, p_temp_file_name);
    exit(255);
  }
}

void xmodem_receive(void) {
  create_temp_file();

  serial_purge_buffers(port_number);

  serial_write_char('C');
  printf("Sent C\r\n");

  while (true) {
    if (!read_single_char()) {
    nak_and_retry:
      printf("Purging...\r\n");
      serial_purge_buffers(port_number);
      serial_write_char(NAK);
      retries++;

      if (retries > MAX_RETRIES) {
        printf("Too many retries\r\n");
        goto aborted;
      }

      continue;
    }

    if (block[0] == EOT) {
      printf("Received EOT\r\n");
      serial_write_char(ACK);
      break;
    }

    if (block[0] != SOH) {
      printf("Expected SOH.  Received %02X\r\n", block[0]);
      goto nak_and_retry;
    }

    read_block();

    if (block[0] == CAN && block[1] == CAN && block[2] == CAN) {
      printf("Received CANs\r\n");
      serial_write_char(ACK);
      goto aborted;
    }

    if (block[1] < block_number && block[2] == (uint8_t)~block[1]) {
      // received a duplicate pck - lets just move on
      printf("Received duplicate block %d\r\n", block[1]);
      serial_write_char(ACK);
      retries = 0;
      continue;
    }

    if (block[1] != block_number) {
      printf("Expected block %d.  Received %d\r\n", block_number, block[1]);
      goto nak_and_retry;
    }

    if (block[2] != (uint8_t)~block_number) {
      printf("Expected block complement %02X.  Received %02X\r\n", (uint8_t)~block_number, block[2]);
      goto nak_and_retry;
    }

    if (!check_crc()) {
      printf("CRC error\r\n");
      goto nak_and_retry;
    }

    printf("Received block %d OK\r\n", block_number);
    write_block_to_file();
    serial_write_char(ACK);
    block_number++;
    retries = 0;
  }

  msxdosCloseFile(fptr);
  msxdosDeleteFile(p_file_name);
  msxdosRenameFile(p_file_name, p_temp_file_name);

  return;

aborted:
  msxdosCloseFile(fptr);
  msxdosDeleteFile(p_temp_file_name);
}
