
#ifndef __CH376
#define __CH376

#include "ch376inc.h"
#include <stdlib.h>

typedef struct {
  uint8_t number;
  uint8_t toggle;
  uint8_t max_packet_size;
} endpoint_param;

typedef enum {
  USB_ERR_OK                          = 0,
  USB_ERR_NAK                         = 1,
  USB_ERR_STALL                       = 2,
  USB_ERR_TIMEOUT                     = 3,
  USB_ERR_DATA_ERROR                  = 4,
  USB_ERR_NO_DEVICE                   = 5,
  USB_ERR_PANIC_BUTTON_PRESSED        = 6,
  USB_TOKEN_OUT_OF_SYNC               = 7,
  USB_ERR_UNEXPECTED_STATUS_FROM_HOST = 8,
  USB_ERR_CODE_EXCEPTION              = 9,
  USB_ERR_MEDIA_CHANGED               = 10,
  USB_ERR_MEDIA_NOT_PRESENT           = 11,
  USB_ERR_CH376_BLOCKED               = 12,
  USB_ERR_MAX                         = 13,
  USB_ERR_OTHER                       = 14,
  USB_ERR_DISK_READ                   = 0x1D,
  USB_ERR_DISK_WRITE                  = 0x1E,
  USB_FILERR_MIN                      = 0x41,
  USB_ERR_OPEN_DIR                    = 0x41,
  USB_ERR_MISS_FILE                   = 0x42,
  USB_FILERR_MAX                      = 0xB4
} usb_error;

typedef enum { CH_NAK_RETRY_DONT = 0b00, CH_NAK_RETRY_INDEFINITE = 0b10, CH_NAK_RETRY_3S = 0b11 } ch_nak_retry_type;

#define CH_MODE_HOST_RESET 7
#define CH_MODE_HOST       6

typedef enum _ch376_pid {
  CH_PID_SETUP = DEF_USB_PID_SETUP,
  CH_PID_IN    = DEF_USB_PID_IN,
  CH_PID_OUT   = DEF_USB_PID_OUT
} ch376_pid;

__sfr __at 0x84 CH376_DATA_PORT;
__sfr __at 0x85 CH376_COMMAND_PORT;

extern void           ch_command(const uint8_t command) __z88dk_fastcall;
extern usb_error      ch_get_status();
extern usb_error      ch_long_wait_int_and_get_status();
extern usb_error      ch_short_wait_int_and_get_status();
extern uint8_t *      ch_read_data(uint8_t *buffer, uint16_t buffer_size, int8_t *const amount_received);
extern void           ch376_reset();
extern uint8_t        ch376_probe();
extern void           ch376_set_usb_mode(const uint8_t mode) __z88dk_fastcall;
extern uint8_t        ch376_get_firmware_version();
extern const uint8_t *ch_write_data(const uint8_t *buffer, uint8_t length);

extern usb_error ch_data_in_transfer(uint8_t *buffer, int16_t data_length, endpoint_param *const endpoint);

extern usb_error ch_data_out_transfer(const uint8_t *buffer, int16_t buffer_length, endpoint_param *const endpoint);

inline void ch_configure_nak_retry(const ch_nak_retry_type retry, const uint8_t number_of_retries) {
  ch_command(CH_CMD_WRITE_VAR8);
  CH376_DATA_PORT = CH_VAR_RETRY_TIMES;
  CH376_DATA_PORT = retry << 6 | (number_of_retries & 0x1F);
}

#define ch_configure_nak_retry_indefinite() ch_configure_nak_retry(CH_NAK_RETRY_INDEFINITE, 0x1F)
#define ch_configure_nak_retry_disable()    ch_configure_nak_retry(CH_NAK_RETRY_DONT, 0x1F)
#define ch_configure_nak_retry_3s()         ch_configure_nak_retry(CH_NAK_RETRY_3S, 0x1F)

extern void ch_issue_token_setup();
extern void ch_issue_token_out_ep0();
extern void ch_issue_token_in_ep0();

#endif
