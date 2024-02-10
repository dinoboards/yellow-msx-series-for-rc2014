#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include <stdint.h>

extern void xmfs_create(const char *file_name);
extern void xmfs_ready_block(const uint8_t const *block); // overwrite pending write RAM
extern void xmfs_write_block(const uint8_t length);       // write length bytes from pending RAM store to disk file
extern void xmfs_close(void);                             // flush buffer, rename temp to real file, etc.
extern void xmfs_abort(void);                             // delete temp file, etc.

#endif
