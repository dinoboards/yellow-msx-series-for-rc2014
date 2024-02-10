#include "file_storage.h"
#include "buffer.h"
#include <libgen.h>
#include <msxdos.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static uint16_t    write_buffer_length = 0;
static uint8_t     error;
static uint8_t     fptr = 0;
static const char *p_file_name;
static char        p_temp_file_name[256];
static char        file_name_buffer[256];
static char       *pPath;
static uint16_t    error_handle;
static bool        block_pending = false;

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

void xmfs_create(const char *file_name) {
  p_file_name         = file_name;
  write_buffer_length = 0;
  fptr                = 0;
  block_pending       = false;

  memset(write_buffer, 0, sizeof(write_buffer));

  create_temp_file();
}

void xmfs_ready_block(const uint8_t const *block) {
  memcpy(write_buffer + write_buffer_length, block, DATA_SIZE);
  block_pending = true;
}

void xmfs_write_block(const uint8_t length) {
  if (!block_pending)
    return;

  write_buffer_length += length;
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

void xmfs_close(void) {
  flush_write_buffer();
  msxdosCloseFile(fptr);
  msxdosDeleteFile(p_file_name);
  msxdosRenameFile(p_file_name, p_temp_file_name);
}

void xmfs_abort(void) {
  msxdosCloseFile(fptr);
  msxdosDeleteFile(p_temp_file_name);
}
