#include "receive.h"
#include "arguments.h"
#include "buffer.h"
#include "crc16.h"
#include "file_storage.h"
#include <delay.h>
#include <extbio/serial-helpers.h>
#include <extbio/serial.h>
#include <msxbios.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strnstr.h>

#define SOH   0x01 // Start of Header
#define EOT   0x04 // End of Transmission
#define ACK   0x06 // Acknowledge
#define NAK   0x15 // Not Acknowledge
#define CAN   0x18 // Cancel
#define CTRLZ 0x1A // EOF
#define RS    0x1E // Start of Extended Info Packet

#define MAX_RETRIES 4

const unsigned char rotatingChar[4][5] = {STR_VERT_BAR CURSOR_MOVE_LEFT, STR_FOWARD_SLASH CURSOR_MOVE_LEFT,
                                          STR_DASH CURSOR_MOVE_LEFT, STR_BACK_SLASH CURSOR_MOVE_LEFT};

static uint8_t  rotatingIndex   = 0;
static uint32_t total_file_size = 0;
uint8_t         block[PACKET_SIZE];
static uint8_t  block_number = 1;
static uint8_t  retries      = 0;
static uint8_t  waiting_for_info_packet;
static uint32_t actual_file_size;
static uint8_t  block_size;

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
  for (uint8_t i = 0; i < 3; i++) {
    const uint8_t r = serial_demand_read(port_number, block, &actual_size, 2000);

    if (r == 0 && actual_size == 1)
      return true;

    printf(rotatingChar[rotatingIndex]);
    rotatingIndex = (rotatingIndex + 1) & 3;
  }

  return false;
}

static uint8_t read_block(void) {
  uint16_t      actual_size = PACKET_SIZE - 1;
  const uint8_t r           = serial_demand_read(port_number, block + 1, &actual_size, 3000);
  return r == 0;
}

/**
 * @brief retrieve a file from the serial port
 *
 * global: port_number is the COM port to use
 *
 * @param _p_file_name the name of the file to be saved
 * @param expect_info_packet if true, expect an additional packet containing file info - ESP8266 specific
 * @return uint8_t 0 if ok, 1 if error
 */
uint8_t xmodem_receive(const char *p_file_name, uint32_t *const retrieved_bytes, const uint8_t expect_info_packet) {
  rotatingIndex           = 0;
  total_file_size         = 0;
  block_number            = 1;
  retries                 = 0;
  waiting_for_info_packet = false;
  block_size              = DATA_SIZE;

  // serial_purge_buffers(port_number);

  printf(CURSOR_OFF);
  printf(rotatingChar[rotatingIndex]);
  rotatingIndex = (rotatingIndex + 1) & 3;

  if (p_file_name)
    xmfs_create(p_file_name);

  serial_write_char_h('C');

  while (true) {
    if (!read_single_char()) {
    nak_and_retry:
      // serial_purge_buffers(port_number);
      serial_write_char_h(NAK);
      retries++;

      if (retries > MAX_RETRIES) {
        printf("Too many errors\r\n");
        goto aborted;
      }

      continue;
    }

    if (block[0] == EOT) {
      serial_write_char_h(ACK);
      retries = 0;
      if (expect_info_packet) {
        waiting_for_info_packet = true;
        block_number            = 0;
        continue;
      } else
        break;
    }

    if (!waiting_for_info_packet && block[0] != SOH)
      goto nak_and_retry;

    if (waiting_for_info_packet && block[0] != RS)
      goto nak_and_retry;

    read_block();

    if (block[0] == CAN && block[1] == CAN && block[2] == CAN) {
      serial_write_char_h(ACK);
      goto aborted;
    }

    if (!waiting_for_info_packet && block[1] < block_number && block[2] == (uint8_t)~block[1]) {
      // ignore a previously received duplicated packet
      serial_write_char_h(ACK);
      retries = 0;
      continue;
    }

    if (block[1] != block_number)
      goto nak_and_retry;

    if (block[2] != (uint8_t)~block_number)
      goto nak_and_retry;

    if (!check_crc())
      goto nak_and_retry;

    printf(rotatingChar[rotatingIndex]);
    rotatingIndex = (rotatingIndex + 1) & 3;

    if (waiting_for_info_packet) {
      serial_write_char_h(CAN);

      const char *lengthPtr = strnstr(block + 3, "LEN=", 128);
      if (lengthPtr) {
        const uint32_t length = atol(lengthPtr + 4);
        block_size            = length - (total_file_size - DATA_SIZE);
        total_file_size       = length;
      }
      break;
    }

    if (p_file_name) {
      xmfs_write_block(block_size);
      xmfs_ready_block(block + 3);

    } else {
      memcpy(write_buffer, block + 3, DATA_SIZE);
    }

    total_file_size += DATA_SIZE;
    serial_write_char_h(ACK);
    block_number++;
    retries = 0;
  }

  if (p_file_name) {
    xmfs_write_block(block_size);
    xmfs_close();
  }

  *retrieved_bytes = total_file_size;

  printf(" " CURSOR_MOVE_LEFT CURSOR_ON);
  return 0;

aborted:
  if (p_file_name)
    xmfs_abort();

  *retrieved_bytes = 0;
  printf(" " CURSOR_MOVE_LEFT CURSOR_ON);

  return 1;
}
