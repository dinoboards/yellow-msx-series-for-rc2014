#include <system_vars.h>
#include <delay.h>
#include <string.h>
#include "print.h"
#include "work-area.h"
#include <stdbool.h>

__sfr __at 0x84 CH376_DATA_PORT;
__sfr __at 0x85 CH376_COMMAND_PORT;


#define CH_CMD_GET_IC_VER   0x01
#define CH_CMD_RESET_ALL    0x05
#define CH_CMD_CHECK_EXIST  0x06
#define CH_CMD_SET_USB_ADDR 0x13
#define CH_CMD_SET_USB_MODE 0x15
#define CH_CMD_SET_RETRY    0x0B
#define CH_CMD_WR_HOST_DATA 0x2C
#define CH_CMD_ISSUE_TKN_X  0x4E


#define CH_MODE_HOST_RESET  7
#define CH_MODE_HOST        6

// return codes
#define CH_ST_RET_SUCCESS         0x51
#define CH_ST_RET_ABORT           0x5F

// CH376 result codes
#define CH_USB_INT_SUCCESS        0x14


typedef enum _ch376_pid {
  CH_PID_SETUP  = 0x0D,
  CH_PID_IN     = 0x09,
  CH_PID_OUT    = 0x01
} ch376_pid;

inline void ch376_reset() {
  CH376_COMMAND_PORT = CH_CMD_RESET_ALL;
  delay(60);
}

uint8_t* ch_write_data(uint8_t* buffer, uint8_t length) {
  CH376_COMMAND_PORT = CH_CMD_WR_HOST_DATA;
  CH376_DATA_PORT = length;

  while(length > 0)
    CH376_DATA_PORT = *buffer++;

  return buffer;
}

void ch_issue_token(uint8_t endpoint, ch376_pid pid, uint8_t toggle_bit) {
  CH376_COMMAND_PORT = CH_CMD_ISSUE_TKN_X;
  CH376_DATA_PORT = toggle_bit;
  CH376_DATA_PORT = endpoint << 4 | pid;
}

uint8_t hw_control_transfer(uint8_t *setupPacket, uint8_t* buffer, uint8_t device_address, uint8_t max_packet_size, uint16_t* amount_transferred) {

  CH376_COMMAND_PORT = CH_CMD_SET_USB_ADDR;
  CH376_DATA_PORT = device_address;

  const uint8_t *xx = ch_write_data(setupPacket, 8);

  ch_issue_token(0, CH_PID_SETUP, 0);

  (void)buffer;
  (void)max_packet_size;
  (void)amount_transferred;
  return 99;
}

uint8_t ch_get_device_descriptor(const work_area *work_area, uint8_t *buffer, uint8_t device_address, uint16_t* amount_transferred) {

  const uint8_t response = hw_control_transfer(work_area->ch376.usb_descriptor_blocks.cmd_get_device_descriptor, buffer, device_address, 8, amount_transferred);

  return (response == CH_USB_INT_SUCCESS);
}

uint8_t hw_get_descriptors(const work_area *work_area, uint8_t* buffer, uint8_t device_address) {
  uint16_t amount_transferred;
  return ch_get_device_descriptor(work_area, buffer, device_address, &amount_transferred);
}

uint8_t fn_connect(work_area *work_area) {
  const uint8_t max_device_address = work_area->ch376.max_device_address;
  if (max_device_address != 0)
    return max_device_address;

  work_area->ch376.max_device_address = 1;

  if (!hw_get_descriptors(work_area, work_area->ch376.usb_descriptor, 0))
    return false;

  return false;
}

/* =============================================================================

  Check if the USB host controller hardware is operational

  Returns:
    1 is operation, 0 if not

============================================================================= */
inline uint8_t ch376_test() {
  CH376_COMMAND_PORT = CH_CMD_CHECK_EXIST;
  CH376_DATA_PORT = (uint8_t)~0x34;
  if (CH376_DATA_PORT != 0x34)
    return false;

  CH376_COMMAND_PORT = CH_CMD_CHECK_EXIST;
  CH376_DATA_PORT = (uint8_t)~0x89;
  return CH376_DATA_PORT == 0x89;
}

/* =============================================================================

  Retrieve the CH376 chip version

  Returns:
    The chip version
============================================================================= */
inline uint8_t ch376_get_firmware_version() {
  CH376_COMMAND_PORT = CH_CMD_GET_IC_VER;
  return CH376_DATA_PORT & 0x1f;
}

/* =============================================================================

  Set the CH376 USB MODE

  Returns:
    0 -> OK, 1 -> ERROR
============================================================================= */
uint8_t ch376_set_usb_mode(uint8_t mode) __z88dk_fastcall {
  CH376_COMMAND_PORT = CH_CMD_SET_USB_MODE;
  CH376_DATA_PORT = mode;

  uint8_t count = 255;
  while( CH376_DATA_PORT != CH_ST_RET_SUCCESS && --count != 0)
    ;

  return count != 0;
}

inline void hw_configure_nak_retry() {
  CH376_COMMAND_PORT = CH_CMD_SET_RETRY;
  CH376_DATA_PORT = 0x25;
  CH376_DATA_PORT = 0x8F;   // Retry NAKs indefinitely (default)
}

inline uint8_t usb_host_bus_reset() {
  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60/4);

  ch376_set_usb_mode(CH_MODE_HOST_RESET);
  delay(60/2);

  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60/4);

  hw_configure_nak_retry();

  return true;
}

#define target_device_address 0
#define configuration_id 0
#define string_id 0
#define config_descriptor_size 9
#define alternate_setting 0
#define packet_filter 0
#define control_interface_id 0

#define report_id 0
#define duration 0x80
#define interface_id 0
#define protocol_id 0

#define hub_descriptor_size 0
#define feature_selector 0
#define port 0
#define value 0

#define storage_interface_id 0

const _usb_descriptor_blocks usb_descriptor_blocks_templates = {
  .cmd_get_device_descriptor   = {0x80, 6, 0, 1, 0, 0, 18, 0},
  .cmd_set_address             = {0x00, 0x05, target_device_address, 0, 0, 0, 0, 0},
  .cmd_set_configuration       = {0x00, 0x09, configuration_id, 0, 0, 0, 0, 0},
  .cmd_get_string              = {0x80, 6, string_id, 3, 0, 0, 255, 0},
  .cmd_get_config_descriptor   = {0x80, 6, configuration_id, 2, 0, 0, config_descriptor_size, 0},
  .cmd_set_interface           = {0x01, 11, alternate_setting, 0, interface_id, 0, 0, 0},
  .cmd_set_packet_filter       = {0b00100001, 0x43, packet_filter, 0, control_interface_id, 0, 0, 0},
  .cmd_set_idle                = {0x21, 0x0a, report_id, duration, interface_id, 0, 0, 0},
  .cmd_set_protocol            = {0x21, 0x0b, protocol_id, 0, interface_id, 0, 0, 0},
  .reserved                    = {{0}},
  .cmd_get_hub_descriptor      = {0b10100000, 6, 0, 0x29, 0, 0, hub_descriptor_size, 0},
  .cmd_set_hub_port_feature    = {0b00100011, 3, feature_selector, 0, port, value, 0, 0},
  .cmd_get_hub_port_status     = {0b10100011, 0, 0, 0, port, 0, 4, 0},
  .cmd_get_max_luns            = {0b10100001, 0b11111110, 0, 0, storage_interface_id, 0, 1, 0},
  .cmd_mass_storage_reset      = {0b00100001, 0b11111111, 0, 0, storage_interface_id, 0, 0, 0}
};

void initialise_work_area(work_area *p) {
  memcpy(&p->ch376.usb_descriptor_blocks, &usb_descriptor_blocks_templates, sizeof(usb_descriptor_blocks_templates));
}

uint8_t usb_host_init() {
  work_area *p = get_work_area();
  xprintf("usb_host_init %p\r\n", p);

  initialise_work_area(p);

  ch376_reset();

  if (!ch376_test()) {
    xprintf("CH376:           NOT PRESENT\r\n");
    return false;
  }

  p->ch376.present = true;
  const uint8_t ver = ch376_get_firmware_version();
  xprintf("CH376:           PRESENT (VER %d)\r\n", ver);

  usb_host_bus_reset();

  // fn_connect(p);

  return true;
}
