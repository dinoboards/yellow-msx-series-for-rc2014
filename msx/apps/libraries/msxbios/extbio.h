#ifndef __EXTBIO
#define __EXTBIO

#include <stdlib.h>

typedef struct {
  char     character_length;    // Character length '5'-'8'
  char     parity;              // Parity 'E','O','I','N'
  char     stop_bits;           // Stop bits '1','2','3'
  char     xon_xoff_control;    // XON/XOFF controll 'X','N'
  char     handshake;           // CTR-RTS hand shake 'H','N'
  char     autoLineFeedSend;    // Auto LF for receive 'A','N'
  char     autoLineFeedReceive; // Auto LF for send 'A','N'
  char     si_so_control;       // SI/SO control 'S','N'
  uint16_t receive_baud;        // 50-19200
  uint16_t send_baud;           // 50-19200
  uint8_t  timeout_counter;     // Time out counter 0-255
} rs232_init_params;

typedef struct {
  uint8_t slot_id;
  void   *jump_table;
  uint8_t reserved;
} rs232_extbio_info;

typedef struct {
  uint8_t slot_id;
  void   *jump_table;
  uint8_t number_of_free_segments;
  uint8_t number_of_segments;
  uint8_t reserved[3];
} memmap_extbio_info;

typedef struct {
  uint8_t deviceId;
  uint8_t _reserved;
} extbio_device_table;

extern void                 extbio_get_dev_info_table(uint8_t device_id, void *info_table);
extern extbio_device_table *extbio_get_dev(extbio_device_table *table) __z88dk_fastcall;

typedef struct {
  uint8_t  call_opcode;
  uint16_t address;
} jump_table_entry;

// UNAPI
extern jump_table_entry *unapi_get_ram_helper(void *reduced_mapper_table, uint8_t *number_of_entries) __sdcccall(1);

// RC2014 DRIVER

#define SIO_CLK_307200  1
#define SIO_CLK_614400  2
#define SIO_CLK_921600  3
#define SIO_CLK_1228800 4
#define SIO_CLK_2457600 5
#define SIO_CLK_3686400 6

#define FOSSIL_FOR_SIO2 0
#define FOSSIL_FOR_FTDI 1

extern uint8_t  extbio_rc2014_get_version(void);
extern void    *extbio_fossil_install(const uint16_t device_type) __z88dk_fastcall;
extern uint8_t  extbio_sio_get_clock(void);
extern uint8_t  extbio_sio_set_clock(const uint8_t new_code) __z88dk_fastcall;
extern uint8_t  extbio_sio_get_present(void);
extern uint16_t extbio_rc2014_hello(void);

// RS232 DRIVER
#define RS232_RAW_MODE 4

extern void     rs232_link(rs232_extbio_info *p) __z88dk_fastcall;
extern uint8_t  rs232_init(rs232_init_params *) __z88dk_fastcall;
extern void     rs232_open(uint8_t mode, uint8_t buffer_length, uint8_t *buffer);
extern void     rs232_close(void);
extern uint8_t  rs232_sndchr(const char ch) __z88dk_fastcall;
extern uint16_t rs232_getchr(void);
extern uint16_t rs232_loc(void);

// memory mapper
extern void memmap_link(memmap_extbio_info *p) __z88dk_fastcall;

#define MEMMAP_USER     0
#define MEMMAP_SYSTEM   1
#define MEMAP_MAIN_SLOT 0

/**
 * @brief allocate a 16K memory segment
 *
 * @param allocation_type - MEMMAP_USER or MEMMAP_SYSTEM
 * @param mapper_slot - slot to use, 0 for main slot
 * @param allocated_segment - returns the allocated segment id
 * @param allocated_slot - returns the slot number
 * @return uint8_t - 0 if success, 1 if failed
 *
 * mapper_slot: 0 for main Mapper or Mapper slot in the form FxxxSSPP
 *   If xxx=000, allocate a segment to the specified slot
 *   If xxx=001, allocate a segment in another slot than the one specified
 *   If xxx=010, allocate a segment to the specified slot if free, otherwise in another slot
 *   If xxx=011, allocate a segment in another slot than the one specified if free otherwise try in specified slot
 */
extern uint8_t
memmap_allocate_segment(uint8_t allocation_type, uint8_t mapper_slot, uint8_t *allocated_segment, uint8_t *allocated_slot);
extern uint8_t memmap_put_page(uint16_t address, uint8_t segment) __sdcccall(1);
extern uint8_t memmap_get_page(uint16_t address) __z88dk_fastcall;
extern uint8_t memmap_get_page_0(void);
extern void    memmap_put_page_0(uint8_t segment) __z88dk_fastcall;
extern uint8_t memmap_get_page_1(void);
extern uint8_t memmap_put_page_1(uint8_t segment) __z88dk_fastcall;
extern uint8_t memmap_get_page_2(void);
extern uint8_t memmap_put_page_2(uint8_t segment) __z88dk_fastcall;
extern uint8_t memmap_get_page_3(void);
extern void    memmap_put_page_3(uint8_t segment) __z88dk_fastcall;

#endif
