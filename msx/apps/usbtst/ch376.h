
#ifndef __CH376
#define __CH376

#include "ch376inc.h"
#include <stdlib.h>

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
  USB_ERR_CH376_TIMEOUT               = 13,
  USB_ERR_FAIL                        = 14,
  USB_ERR_MAX                         = 14,
  USB_ERR_OTHER                       = 15,
  USB_ERR_DISK_READ                   = 0x1D,
  USB_ERR_DISK_WRITE                  = 0x1E,
  USB_FILERR_MIN                      = 0x41,
  USB_ERR_OPEN_DIR                    = 0x41,
  USB_ERR_MISS_FILE                   = 0x42,
  USB_FILERR_MAX                      = 0xB4,
  USB_INT_CONNECT                     = 0x81
} usb_error;

typedef enum { CH_NAK_RETRY_DONT = 0b00, CH_NAK_RETRY_INDEFINITE = 0b10, CH_NAK_RETRY_3S = 0b11 } ch_nak_retry_type;

typedef enum { USB_IS_FLOPPY = 1, USB_IS_MASS_STORAGE = 2, USB_IS_CDC = 3, USB_IS_HUB = 128 } usb_device_type;

typedef enum { ENDPOINT_BULK_OUT = 0, ENDPOINT_BULK_IN = 1, ENDPOINT_INTERRUPT_IN = 2 } usb_endpoint_type;

#define CHECK(fn, ...)                                                                                                             \
  {                                                                                                                                \
    result = fn;                                                                                                                   \
    if (result != USB_ERR_OK && result != USB_ERR_STALL) {                                                                         \
      printf("Error: %s:%d %d\r\n", __FILE__, __LINE__, result);                                                                   \
      return result;                                                                                                               \
    }                                                                                                                              \
  }

typedef struct {
  uint8_t toggle : 1;
  uint8_t number : 4;
  uint8_t max_packet_size;
} endpoint_param;

typedef struct {
  uint8_t max_packet_size;
  uint8_t value;
  uint8_t interface_number;
  uint8_t tag;
  uint8_t address;
} device_config;

typedef struct {
  usb_device_type type; // floppy or mass storage
  device_config   config;
  endpoint_param  endpoints[3]; // bulk in/out and interrupt
} storage_device_config;

#define CH_SPEED_FULL     0 /* 12Mbps full speed FullSpeed ​​(default value) */
#define CH_SPEED_LOW_FREQ 1 /*1.5Mbps (modify frequency only) */
#define CH_SPEED_LOW      2 /*=1.5Mbps low speed LowSpeed*/

#define CH_MODE_HOST_RESET 7
#define CH_MODE_HOST       6

typedef enum _ch376_pid { CH_PID_SETUP = DEF_USB_PID_SETUP, CH_PID_IN = DEF_USB_PID_IN, CH_PID_OUT = DEF_USB_PID_OUT } ch376_pid;

__sfr __at 0x88 CH376_DATA_PORT;
__sfr __at 0x89 CH376_COMMAND_PORT;

extern void delay_short();
extern void delay_medium();

extern void           ch_command(const uint8_t command) __z88dk_fastcall;
extern usb_error      ch_get_status();
extern usb_error      ch_long_wait_int_and_get_status();
extern usb_error      ch_short_wait_int_and_get_status();
extern usb_error      ch_very_short_wait_int_and_get_status();
extern uint8_t        ch_read_data(uint8_t *buffer, uint16_t buffer_size);
extern void           ch_cmd_reset_all();
extern uint8_t        ch_probe();
extern usb_error      ch_cmd_set_usb_mode(const uint8_t mode) __z88dk_fastcall;
extern uint8_t        ch_cmd_get_ic_version();
extern const uint8_t *ch_write_data(const uint8_t *buffer, uint8_t length);

extern void ch_set_usb_address(const uint8_t device_address) __z88dk_fastcall;

extern usb_error ch_control_transfer_request_descriptor(const uint8_t descriptor_type) __z88dk_fastcall;
extern usb_error ch_control_transfer_set_address(const uint8_t device_address) __z88dk_fastcall;
extern usb_error ch_control_transfer_set_config(const uint8_t config_value) __z88dk_fastcall;

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
