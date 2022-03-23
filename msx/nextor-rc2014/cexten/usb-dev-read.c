#include "ch376.h"
#include "scsi.h"
#include "work-area.h"
#include <delay.h>
#include <stdlib.h>
#include <string.h>

#include "print.h"

#define NEXTOR_ERR_OK    0x00
#define NEXTOR_ERR_NCOMP 0xFF
#define NEXTOR_ERR_WRERR 0xFE
#define NEXTOR_ERR_DISK  0xFD
#define NEXTOR_ERR_NRDY  0xFC
#define NEXTOR_ERR_DATA  0xFA
#define NEXTOR_ERR_RNF   0xF9
#define NEXTOR_ERR_WPROT 0xF8
#define NEXTOR_ERR_UFORM 0xF7
#define NEXTOR_ERR_SEEK  0xF3
#define NEXTOR_ERR_IFORM 0xF0
#define NEXTOR_ERR_IDEVL 0xB5
#define NEXTOR_ERR_IPARM 0x8B

uint8_t scsi_read(const uint8_t number_of_sectors, const uint32_t sector_number, uint8_t *const buffer) {
  uint8_t          result = 101;
  work_area *const p      = get_work_area();

  // for (uint8_t retries = 4; retries != 0; retries--) {
    _scsi_packet_read_write cmd_packet;
    cmd_packet = scsi_packet_read;

    cmd_packet.transfer_len[1] = number_of_sectors;
    cmd_packet.lba[0]          = sector_number >> 24;
    cmd_packet.lba[1]          = sector_number >> 16;
    cmd_packet.lba[2]          = sector_number >> 8;
    cmd_packet.lba[3]          = sector_number;

    if ((result = do_scsi_cmd_with_reset_retry(&p->ch376,
                              p->ch376.storage_device_info.device_address,
                              0,
                              sizeof(_scsi_packet_read_write),
                              (number_of_sectors)*512,
                              (uint8_t*)&cmd_packet,
                              buffer,
                              false)) == CH_USB_INT_SUCCESS)
      return result;

    // delay(2);
  // }

  return result;
}

uint8_t usb_dev_read(const uint8_t  lun,
                     const uint8_t  number_sectors_to_read,
                     uint32_t       sector_number,
                     uint8_t *      buffer,
                     uint8_t *const number_of_sectors_read) {

  (void)lun;
  uint8_t        result;
  uint8_t *const p = buffer;

  for (uint8_t index = 0; index < number_sectors_to_read; index++) {
    if ((result = scsi_read(1, sector_number, buffer)) != CH_USB_INT_SUCCESS) {
      yprintf(60, " ER %x ", result);
      *number_of_sectors_read = index;
      return NEXTOR_ERR_DISK;
    }
    sector_number++;
    buffer += 512;
  }

  *number_of_sectors_read = number_sectors_to_read;
  return NEXTOR_ERR_OK;
}
