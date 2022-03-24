#include "ch376.h"
#include "debuggin.h"
#include "print.h"
#include "scsi.h"
#include "work-area.h"
#include <delay.h>
#include <stdbool.h>
#include <string.h>
#include <system_vars.h>

__sfr __at 0x84 CH376_DATA_PORT;
__sfr __at 0x85 CH376_COMMAND_PORT;

const void setCommand(const uint8_t command) __z88dk_fastcall {
  CH376_COMMAND_PORT = command;
  for (uint8_t i = 255; i != 0; i--)
    ;
}

inline void ch376_reset() {
  delay(30);
  setCommand(CH_CMD_RESET_ALL);
  delay(30);
}

const uint8_t *ch_write_data(const uint8_t *buffer, uint8_t length) {
  setCommand(CH_CMD_WR_HOST_DATA);
  CH376_DATA_PORT = length;

  while (length-- != 0) {
    CH376_DATA_PORT = *buffer++;
  }

  return buffer;
}

// endpoint => e
// pid => B
// toggle => A (bit 7 for in, bit 6 for out)
void ch_issue_token(const uint8_t endpoint, const ch376_pid pid, const uint8_t toggle_bits) {
  setCommand(CH_CMD_ISSUE_TKN_X);
  CH376_DATA_PORT = toggle_bits;
  CH376_DATA_PORT = endpoint << 4 | pid;
}

uint8_t ch_wait_int_and_get_result() {
  uint8_t counter = 255;
  while ((CH376_DATA_PORT & 0x80) && --counter != 0)
    ;

  setCommand(CH_CMD_GET_STATUS);
  return CH376_DATA_PORT;
}

// buffer => hl
// C -> amount_received
// returned -> hl
uint8_t *ch_read_data(uint8_t *buffer, uint16_t buffer_size, int8_t *const amount_received) {
  setCommand(CH_CMD_RD_USB_DATA0);
  uint8_t count    = CH376_DATA_PORT;
  *amount_received = count;
  uint8_t extra    = 0;

  if (count > buffer_size) {
    extra = count - buffer_size;

    yprintf(0, "Y (%d,", count);
    xprintf("%d,", buffer_size);
    xprintf("%d)", extra);

    count = buffer_size;
  }

  while (count-- != 0)
    *buffer++ = CH376_DATA_PORT;

  while (extra-- != 0) // do we need to flush buffer?
    CH376_DATA_PORT;

  return buffer;
}
// buffer -> hl
// data_length -> bc
// device_address -> a
// max_packet_size -> d
// endpoint -> e
// *toggle -> Cy
// *amount_received -> BC
uint8_t ch_data_in_transfer(uint8_t *       buffer,
                            int16_t         data_length,
                            const uint8_t   max_packet_size,
                            const uint8_t   endpoint,
                            uint16_t *const amount_received,
                            uint8_t *const  toggle) {
  uint8_t count;
  uint8_t result;
  do {
    ch_issue_token(endpoint, CH_PID_IN, *toggle ? 0x80 : 0x00);
    *toggle = ~*toggle;

    if ((result = ch_wait_int_and_get_result()) != CH_USB_INT_SUCCESS)
      return result;

    buffer = ch_read_data(buffer, data_length, &count);
    data_length -= count;
    *amount_received += count;
  } while (data_length > 0 && count <= max_packet_size);

  return CH_USB_INT_SUCCESS;
}

// buffer -> hl
// data_length -> bc
// device_address -> a
// max_packet_size -> d
// endpoint -> e
// device_address => a
// *toggle -> Cy
// *amount_received -> BC
uint8_t hw_data_in_transfer(uint8_t *       buffer,
                            const uint16_t  data_length,
                            const uint8_t   max_packet_size,
                            const uint8_t   endpoint,
                            const uint8_t   device_address,
                            uint16_t *const amount_received,
                            uint8_t *const  toggle) {

  setCommand(CH_CMD_SET_USB_ADDR);
  CH376_DATA_PORT = device_address;

  return ch_data_in_transfer(buffer, data_length, max_packet_size, endpoint, amount_received, toggle);
}

// buffer => HL
// buffer_length => BC
// max_packet_size => D
// endpoint => E
// *toggle => Cy
uint8_t ch_data_out_transfer(const uint8_t *buffer,
                             int16_t        buffer_length,
                             const uint8_t  max_packet_size,
                             const uint8_t  endpoint,
                             uint8_t *const toggle) {
  uint8_t result;

  while (buffer_length > 0) {
    const uint8_t size = max_packet_size < buffer_length ? max_packet_size : buffer_length;
    buffer             = ch_write_data(buffer, size);
    buffer_length -= size;
    ch_issue_token(endpoint, CH_PID_OUT, *toggle ? 0x40 : 0x00);
    if ((result = ch_wait_int_and_get_result()) != CH_USB_INT_SUCCESS)
      return result;

    *toggle = ~*toggle;
  }

  return CH_USB_INT_SUCCESS;
}
// buffer => HL
// buffer_length => BC
// max_packet_size => D
// endpoint => E
// *toggle => Cy
// device_address -> A
uint8_t hw_data_out_transfer(const uint8_t *buffer,
                             uint16_t       buffer_length,
                             const uint8_t  max_packet_size,
                             const uint8_t  endpoint,
                             const uint8_t  device_address,
                             uint8_t *const toggle) {

  setCommand(CH_CMD_SET_USB_ADDR);
  CH376_DATA_PORT = device_address;

  return ch_data_out_transfer(buffer, buffer_length, max_packet_size, endpoint, toggle);
}

// cmd_packet -> HL
// buffer -> DE
// device_address -> a
// max_packet_size -> b
// *amount_transferred -> bc
uint8_t hw_control_transfer(const usb_descriptor_block *const cmd_packet,
                            uint8_t *const                    buffer,
                            const uint8_t                     device_address,
                            const uint8_t                     max_packet_size,
                            uint16_t *const                   amount_transferred) {
  uint8_t result;
  uint8_t toggle;

retry:
  toggle = 1;
  setCommand(CH_CMD_SET_USB_ADDR);
  CH376_DATA_PORT = device_address;

  ch_write_data((const uint8_t *)cmd_packet, sizeof(usb_descriptor_block));

  ch_issue_token(0, CH_PID_SETUP, 0);

  if ((result = ch_wait_int_and_get_result()) != CH_USB_INT_SUCCESS) {
    yprintf(10, "Err1 (%d) ", result);
    return result;
  }

  const uint8_t transferIn = (cmd_packet->code & 0x80);

  if (transferIn && buffer == 0) {
    yprintf(10, "Err2 ");
    return 98;
  }

  result = transferIn ? ch_data_in_transfer(buffer, cmd_packet->length, max_packet_size, 0, amount_transferred, &toggle)
                      : ch_data_out_transfer(buffer, cmd_packet->length, max_packet_size, 0, &toggle);

  if ((result & 0x2f) == USB_STALL) {
    yprintf(10, "Stall");
    setCommand(CH_CMD_CLR_STALL);
    delay(60 / 4);
    CH376_DATA_PORT = cmd_packet->code & 0x80;

    result = ch_wait_int_and_get_result();
    if (result == CH_USB_INT_SUCCESS)
      goto retry;
  }

  if (result != CH_USB_INT_SUCCESS) {
    yprintf(10, "Err3 (%d) ", result);
    return result;
  }

  if (transferIn)
    ch_issue_token(0, CH_PID_OUT, 0x40);
  else
    ch_issue_token(0, CH_PID_IN, 0x80);

  result = ch_wait_int_and_get_result();

  if (transferIn)
    return CH_USB_INT_SUCCESS;

  setCommand(CH_CMD_RD_USB_DATA0);
  result = CH376_DATA_PORT;
  if (result == 0)
    return CH_USB_INT_SUCCESS;

  return result;
}

uint8_t ch_get_device_descriptor(const work_area *const   work_area,
                                 device_descriptor *const buffer,
                                 const uint8_t            device_address,
                                 uint16_t *const          amount_transferred) {
  *amount_transferred = 0;
  return hw_control_transfer(&work_area->ch376.usb_descriptor_blocks.cmd_get_device_descriptor,
                             (uint8_t *)buffer,
                             device_address,
                             8,
                             amount_transferred);
}

// buffer => hl
// configuration index starting with 0 to DEVICE_DESCRIPTOR.bNumConfigurations => A
// max_packet_size => B
// buffer_size => C
// device_address => D
uint8_t ch_get_config_descriptor(work_area *const         work_area,
                                 config_descriptor *const buffer,
                                 const uint8_t            config_index,
                                 const uint8_t            max_packet_size,
                                 const uint8_t            buffer_size,
                                 const uint8_t            device_address,
                                 uint16_t *const          amount_transferred) {
  *amount_transferred                                                     = 0;
  work_area->ch376.usb_descriptor_blocks.cmd_get_config_descriptor.dat2   = config_index;
  work_area->ch376.usb_descriptor_blocks.cmd_get_config_descriptor.length = (uint16_t)buffer_size;

  return hw_control_transfer(&work_area->ch376.usb_descriptor_blocks.cmd_get_config_descriptor,
                             (uint8_t *)buffer,
                             device_address,
                             max_packet_size,
                             amount_transferred);
}

// config_id => A
// max_packet_size => B
// device_address => D
uint8_t ch_set_configuration(work_area *const work_area,
                             const uint8_t    config_id,
                             const uint8_t    max_packet_size,
                             const uint8_t    device_address,
                             uint16_t *const  amount_transferred) {
  *amount_transferred                                               = 0;
  work_area->ch376.usb_descriptor_blocks.cmd_set_configuration.dat2 = config_id;

  return hw_control_transfer(&work_area->ch376.usb_descriptor_blocks.cmd_set_configuration,
                             (uint8_t *)0,
                             device_address,
                             max_packet_size,
                             amount_transferred);
}

uint8_t scsi_max_luns(work_area *const work_area, uint16_t *const amount_transferred) {
  *amount_transferred = 0;

  work_area->ch376.usb_descriptor_blocks.cmd_get_max_luns.dat4 = work_area->ch376.storage_device_info.interface_id;

  return hw_control_transfer(&work_area->ch376.usb_descriptor_blocks.cmd_get_max_luns,
                             (uint8_t *)&work_area->ch376.scsi_device_info.max_luns,
                             work_area->ch376.storage_device_info.device_address,
                             work_area->ch376.storage_device_info.max_packet_size,
                             amount_transferred);
}

// usb_address => A
// packet_size => B
uint8_t ch_set_address(work_area *const work_area, const uint8_t usb_address, const uint8_t packet_size) {
  uint16_t amount_transferred = 0;

  work_area->ch376.usb_descriptor_blocks.cmd_set_address.dat2 = usb_address;
  return hw_control_transfer(
      &work_area->ch376.usb_descriptor_blocks.cmd_set_address, (uint8_t *)0, 0, packet_size, &amount_transferred);
}

uint8_t hw_get_descriptors(work_area *const work_area, uint8_t *buffer, uint8_t device_address) {
  uint8_t  result;
  uint16_t amount_transferred = 0;

  result = ch_get_device_descriptor(work_area, (device_descriptor *)buffer, device_address, &amount_transferred);

  if (result != CH_USB_INT_SUCCESS) {
    yprintf(25, "X1 (%d)", result);

    return result; // todo try on low speed for device_address 1
  }

  const uint8_t num_configurations = ((device_descriptor *)buffer)->bNumConfigurations;
  const uint8_t max_packet_size    = ((device_descriptor *)buffer)->bMaxPacketSize0;

  if (device_address == 0) {
    if ((result = ch_set_address(work_area, work_area->ch376.max_device_address, max_packet_size)) != CH_USB_INT_SUCCESS) {
      yprintf(15, "X2 (%d)", result);
      return result;
    }
  }

  buffer += sizeof(device_descriptor);

  for (uint8_t config_index = 0; config_index < num_configurations; config_index++) {
    if (device_address == 0) {
      device_address = work_area->ch376.max_device_address;
    }

    if ((result = ch_get_config_descriptor(work_area,
                                           ((config_descriptor *)buffer),
                                           config_index,
                                           max_packet_size,
                                           sizeof(config_descriptor),
                                           device_address,
                                           &amount_transferred)) != CH_USB_INT_SUCCESS) {
      yprintf(15, "X3 (%d)", result);
      return result;
    }

    const uint8_t total_length = ((config_descriptor *)buffer)->wTotalLength;

    if ((result = ch_get_config_descriptor(work_area,
                                           ((config_descriptor *)buffer),
                                           config_index,
                                           max_packet_size,
                                           total_length,
                                           device_address,
                                           &amount_transferred)) != CH_USB_INT_SUCCESS) {
      yprintf(10, "Err4 (%d,%d)", result, amount_transferred);
      return result;
    }

    buffer += total_length;
  }

  return CH_USB_INT_SUCCESS;
}

void check_device_descriptor(work_area *const work_area, const device_descriptor *const buffer) {
  work_area->ch376.search_device_info.num_configs  = buffer->bNumConfigurations;
  work_area->ch376.usb_device_info.max_packet_size = buffer->bMaxPacketSize0;
}

void check_config_descriptor(work_area *work_area, const config_descriptor *buffer) {
  work_area->ch376.search_device_info.num_interfaces = buffer->bNumInterfaces;
  work_area->ch376.usb_device_info.config_id         = buffer->bConfigurationvalue;

  work_area->ch376.search_device_info.num_configs--;
}

void check_interface_descriptor(work_area *work_area, const interface_descriptor *buffer) {
  work_area->ch376.search_device_info.num_endpoints = buffer->bNumEndpoints;

  if (work_area->ch376.search_device_info.wanted_class == buffer->bInterfaceClass) {
    uint8_t wanted_sub_class = work_area->ch376.search_device_info.wanted_sub_class;

    if (wanted_sub_class == 0xff || wanted_sub_class == buffer->bInterfaceSubClass) {
      uint8_t wanted_protocol = work_area->ch376.search_device_info.wanted_protocol;

      if (wanted_protocol == 0xff || wanted_protocol == buffer->bInterfaceProtocol) {
        work_area->ch376.usb_device_info.interface_id = buffer->bInterfaceNumber;
      }
    }
  }

  work_area->ch376.search_device_info.num_interfaces--;
}

void check_endpoint_descriptor(work_area *const work_area, const endpoint_descriptor *const buffer) {
  if ((buffer->bmAttributes & 0b00000011) == 0b00000010) {
    uint8_t endpointAddress = buffer->bEndpointAddress;
    if (endpointAddress & 0b10000000) {
      work_area->ch376.usb_device_info.data_bulk_in_endpoint_id = endpointAddress & 0b01111111;
    } else {
      work_area->ch376.usb_device_info.data_bulk_out_endpoint_id = endpointAddress & 0b01111111;
    }
  }

  work_area->ch376.search_device_info.num_endpoints--;
}

void parse_usb_descriptors(work_area *const work_area) {
  uint8_t  length;
  uint8_t  type;
  uint8_t *buffer = work_area->ch376.usb_descriptor;

loop:
  length = buffer[0];
  type   = buffer[1];

  xprintf("P(%d,%d)", length, type);

  switch (type) {
  case 0:
    // clang-format off
    __asm
      di
      halt
    __endasm;
  // clang-format on
  case 1:
    check_device_descriptor(work_area, (device_descriptor *)buffer);
    break;

  case 2:
    check_config_descriptor(work_area, (config_descriptor *)buffer);
    break;

  case 4:
    check_interface_descriptor(work_area, (interface_descriptor *)buffer);
    break;

  case 5:
    check_endpoint_descriptor(work_area, (endpoint_descriptor *)buffer);
    break;
  }

  if (work_area->ch376.search_device_info.num_configs != 0) {
    buffer += length;
    goto loop;
  }

  if (work_area->ch376.search_device_info.num_interfaces != 0) {
    buffer += length;
    goto loop;
  }

  if (work_area->ch376.search_device_info.num_endpoints != 0) {
    buffer += length;
    goto loop;
  }
}

bool check_descriptor_mass_storage(work_area *const work_area) {
  work_area->ch376.usb_device_info.interface_id        = 0xff;
  work_area->ch376.usb_device_info.config_id           = 0xff;
  work_area->ch376.search_device_info.wanted_class     = 0x8;
  work_area->ch376.search_device_info.wanted_sub_class = 0x6;
  work_area->ch376.search_device_info.wanted_protocol  = 0x50;
  parse_usb_descriptors(work_area);
  return work_area->ch376.usb_device_info.interface_id != 0xff;
}

uint8_t init_storage(work_area *const work_area) {
  uint16_t amount_transferred;
  work_area->ch376.usb_device_info.device_address = work_area->ch376.max_device_address;
  work_area->ch376.storage_device_info            = work_area->ch376.usb_device_info;

  return ch_set_configuration(work_area,
                              work_area->ch376.storage_device_info.config_id,
                              work_area->ch376.storage_device_info.max_packet_size,
                              work_area->ch376.storage_device_info.device_address,
                              &amount_transferred);
}

extern uint32_t usb_scsi_read_capacity();

bool fn_connect(work_area *const work_area) {
  const uint8_t max_device_address = work_area->ch376.max_device_address;
  if (max_device_address != 0)
    return max_device_address;

  work_area->ch376.max_device_address = 1;

  memset(work_area->ch376.usb_descriptor, 0, sizeof(work_area->ch376.usb_descriptor));

  // found usb things?
  if (hw_get_descriptors(work_area, work_area->ch376.usb_descriptor, 0) != CH_USB_INT_SUCCESS) {
    printf("USB:             NOT PRESENT\r\n");
    return false;
  }

  // found usb storage
  uint8_t result = check_descriptor_mass_storage(work_area);
  printf("USB-STORAGE:     ");

  work_area->ch376.initialised = true;

  if (result) {
    if ((result = init_storage(work_area)) != CH_USB_INT_SUCCESS) {
      printf("Err5 %d ", result);
      result = false;
    } else {
      result = true;
    }
  }

  printf(result ? "PRESENT\r\n" : "NOT PRESENT\r\n");

  return result;
}

/* =============================================================================

  Check if the USB host controller hardware is operational

  Returns:
    1 is operation, 0 if not

============================================================================= */
inline uint8_t ch376_test() {
  setCommand(CH_CMD_CHECK_EXIST);
  CH376_DATA_PORT = (uint8_t)~0x34;
  if (CH376_DATA_PORT != 0x34)
    return false;

  setCommand(CH_CMD_CHECK_EXIST);
  CH376_DATA_PORT = (uint8_t)~0x89;
  return CH376_DATA_PORT == 0x89;
}

uint8_t ch376_probe() {
  for (int8_t i = 8; i > 0; i--) {
    if (ch376_test())
      return true;

    delay(5);
  }

  return false;
}

/* =============================================================================

  Retrieve the CH376 chip version

  Returns:
    The chip version
============================================================================= */
inline uint8_t ch376_get_firmware_version() {
  setCommand(CH_CMD_GET_IC_VER);
  return CH376_DATA_PORT & 0x1f;
}

/* =============================================================================

  Set the CH376 USB MODE

  Returns:
    0 -> OK, 1 -> ERROR
============================================================================= */
uint8_t ch376_set_usb_mode(const uint8_t mode) __z88dk_fastcall {
  setCommand(CH_CMD_SET_USB_MODE);
  CH376_DATA_PORT = mode;

  uint8_t count = 255;
  while (CH376_DATA_PORT != CH_ST_RET_SUCCESS && --count != 0)
    ;

  return count != 0;
}

inline void hw_configure_nak_retry() {
  setCommand(CH_CMD_SET_RETRY);
  CH376_DATA_PORT = 0x25;
  CH376_DATA_PORT = 0x8F; // Retry NAKs indefinitely (default)
}

inline uint8_t usb_host_bus_reset() {
  for (int8_t i = 64; i > 0; i++)
    CH376_DATA_PORT;

  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60 / 4);

  ch376_set_usb_mode(CH_MODE_HOST_RESET);
  delay(60 / 2);

  ch376_set_usb_mode(CH_MODE_HOST);
  delay(60 / 4);

  hw_configure_nak_retry();

  setCommand(CH375_CMD_UNLOCK_USB);
  delay(60 / 4);

  return true;
}

#define TARGET_DEVICE_ADDRESS  0
#define CONFIGURATION_ID       0
#define STRING_ID              0
#define CONFIG_DESCRIPTOR_SIZE (sizeof(config_descriptor))
#define ALTERNATE_SETTING      0
#define PACKET_FILTER          0
#define CONTROL_INTERFACE_ID   0

#define PLACEHOLDER_REPORT_ID    0
#define PLACEHOLDER_DURATION     0x80
#define PLACEHOLDER_INTERFACE_ID 0
#define PLACEHOLDER_PROTOCOL_ID  0

#define HUB_DESCRIPTOR_SIZE 0
#define FEATURE_SELECTOR    0
#define PLACEHOLDER_PORT    0
#define PLACEHOLDER_VALUE   0

#define STORAGE_INTERFACE_ID 0

const _usb_descriptor_blocks usb_descriptor_blocks_templates = {
    .cmd_get_device_descriptor = {0x80, 6, 0, 1, 0, 0, 18},
    .cmd_set_address           = {0x00, 0x05, TARGET_DEVICE_ADDRESS, 0, 0, 0, 0},
    .cmd_set_configuration     = {0x00, 0x09, CONFIGURATION_ID, 0, 0, 0, 0},
    .cmd_get_string            = {0x80, 6, STRING_ID, 3, 0, 0, 255},
    .cmd_get_config_descriptor = {0x80, 6, CONFIGURATION_ID, 2, 0, 0, CONFIG_DESCRIPTOR_SIZE},
    .cmd_set_interface         = {0x01, 11, ALTERNATE_SETTING, 0, PLACEHOLDER_INTERFACE_ID, 0, 0},
    .cmd_set_packet_filter     = {0b00100001, 0x43, PACKET_FILTER, 0, CONTROL_INTERFACE_ID, 0, 0},
    .cmd_set_idle              = {0x21, 0x0a, PLACEHOLDER_REPORT_ID, PLACEHOLDER_DURATION, PLACEHOLDER_INTERFACE_ID, 0, 0},
    .cmd_set_protocol          = {0x21, 0x0b, PLACEHOLDER_PROTOCOL_ID, 0, PLACEHOLDER_INTERFACE_ID, 0, 0},
    .reserved                  = {{0}},
    .cmd_get_hub_descriptor    = {0b10100000, 6, 0, 0x29, 0, 0, HUB_DESCRIPTOR_SIZE},
    .cmd_set_hub_port_feature  = {0b00100011, 3, FEATURE_SELECTOR, 0, PLACEHOLDER_PORT, PLACEHOLDER_VALUE, 0},
    .cmd_get_hub_port_status   = {0b10100011, 0, 0, 0, PLACEHOLDER_PORT, 0, 4, 0},
    .cmd_get_max_luns          = {0b10100001, 0b11111110, 0, 0, STORAGE_INTERFACE_ID, 0, 1},
    .cmd_mass_storage_reset    = {0b00100001, 0b11111111, 0, 0, STORAGE_INTERFACE_ID, 0, 0}};

void initialise_work_area(work_area *const p) {
  p->ch376.max_device_address = 0;
  memcpy(&p->ch376.usb_descriptor_blocks, &usb_descriptor_blocks_templates, sizeof(usb_descriptor_blocks_templates));
}

void scsi_init(ch376_work_area *const work_area) {
  work_area->scsi_device_info.tag = 1;
  work_area->storage_device_info.data_bulk_in_endpoint_toggle =
      work_area->storage_device_info.data_bulk_out_endpoint_toggle = 0;
}

_scsi_command_block_wrapper *prepare_cbw(ch376_work_area *const work_area,
                                         const uint8_t          lun,
                                         const uint8_t          cmd_buffer_length,
                                         const uint16_t         send_receive_buffer_length,
                                         uint8_t *const         cmd_buffer) {
  _scsi_command_block_wrapper *const cbw = (_scsi_command_block_wrapper *)&work_area->scsi_device_info.buffer[0];
  memcpy(cbw, &scsi_command_block_wrapper, sizeof(_scsi_command_block_wrapper));
  memcpy(cbw + 1, cmd_buffer, cmd_buffer_length);

  cbw->bCBWLUN                = lun;
  cbw->bCBWCBLength           = cmd_buffer_length;
  cbw->dCBWDataTransferLength = send_receive_buffer_length;
  cbw->dCBWTag[0]             = ++work_area->scsi_device_info.tag;

  return cbw;
}

// device_address => a
// lun => B
// cmd_buffer_length => C
// send_receive_buffer_length => DE
// cmd_buffer => HL
// send_receive_buffer => IX
// send => Cy
uint8_t do_scsi_cmd_with_reset_retry(ch376_work_area *const work_area,
                                     const uint8_t          device_address,
                                     const uint8_t          lun,
                                     const uint8_t          cmd_buffer_length,
                                     const uint16_t         send_receive_buffer_length,
                                     uint8_t *const         cmd_buffer,
                                     uint8_t *const         send_receive_buffer,
                                     bool                   send) {

  uint16_t amount_transferred = 0;

  uint8_t result = do_scsi_cmd(
      work_area, device_address, lun, cmd_buffer_length, send_receive_buffer_length, cmd_buffer, send_receive_buffer, send);

  if (result == CH_USB_INT_SUCCESS)
    return result;

  delay(60 * 2);
  yprintf(0, "XXXXXXXXX (%d) ", send);

  work_area->usb_descriptor_blocks.cmd_mass_storage_reset.dat4 = work_area->storage_device_info.interface_id;

  result = hw_control_transfer(&work_area->usb_descriptor_blocks.cmd_mass_storage_reset,
                               (uint8_t *)0,
                               work_area->storage_device_info.device_address,
                               work_area->storage_device_info.max_packet_size,
                               &amount_transferred);

  xprintf(" %02X, %02X ", result, amount_transferred);

  delay(60 * 2);

  result = do_scsi_cmd(
      work_area, device_address, lun, cmd_buffer_length, send_receive_buffer_length, cmd_buffer, send_receive_buffer, send);

  xprintf(" [[%d]] ", result);

  return result;
}

// device_address => a
// lun => B
// cmd_buffer_length => C
// send_receive_buffer_length => DE
// cmd_buffer => HL
// send_receive_buffer => IX
// send => Cy
uint8_t do_scsi_cmd(ch376_work_area *const work_area,
                    const uint8_t          device_address,
                    const uint8_t          lun,
                    const uint8_t          cmd_buffer_length,
                    const uint16_t         send_receive_buffer_length,
                    uint8_t *const         cmd_buffer,
                    uint8_t *const         send_receive_buffer,
                    bool                   send) {
  uint8_t  result;
  uint16_t amount_received = 0;

  _scsi_command_block_wrapper *const cbw =
      prepare_cbw(work_area, lun, cmd_buffer_length, send_receive_buffer_length, cmd_buffer);

  if (!send)
    cbw->bmCBWFlags = 0x80;

  result = hw_data_out_transfer((uint8_t *)cbw,
                                sizeof(_scsi_command_block_wrapper) + 16,
                                work_area->storage_device_info.max_packet_size,
                                work_area->storage_device_info.data_bulk_out_endpoint_id,
                                device_address,
                                &work_area->storage_device_info.data_bulk_out_endpoint_toggle);

  if (result != CH_USB_INT_SUCCESS) {
    yprintf(10, "Err6 %d ", result);
    return result;
  }

  //_DO_SCSI_CMD_PAYLOAD
  if (cbw->dCBWDataTransferLength != 0) {
    if ((cbw->bmCBWFlags & 0x80) != 0) {

      result = hw_data_in_transfer(send_receive_buffer,
                                   (uint16_t)cbw->dCBWDataTransferLength,
                                   work_area->storage_device_info.max_packet_size,
                                   work_area->storage_device_info.data_bulk_in_endpoint_id,
                                   device_address,
                                   &amount_received,
                                   &work_area->storage_device_info.data_bulk_in_endpoint_toggle);
    } else {
      result = hw_data_out_transfer(send_receive_buffer,
                                    (uint16_t)cbw->dCBWDataTransferLength,
                                    work_area->storage_device_info.max_packet_size,
                                    work_area->storage_device_info.data_bulk_out_endpoint_id,
                                    device_address,
                                    &work_area->storage_device_info.data_bulk_out_endpoint_toggle);
    }

    if (result != CH_USB_INT_SUCCESS) {
      yprintf(10, "Err7 %d ", result);
      return result;
    }
  }

  // _DO_SCSI_CMD_RCV_COMMAND_STATUS_WRAPPER

  // xprintf(" a8 (%p, %d, %d, %d, %d, %p, %p, %d)",
  //   (uint8_t*)work_area->scsi_device_info.csw,
  //   sizeof(_scsi_command_status_wrapper),
  //   work_area->storage_device_info.max_packet_size,
  //   work_area->storage_device_info.data_bulk_in_endpoint_id,
  //   device_address,
  //   &amount_received,
  //   &work_area->storage_device_info.data_bulk_in_endpoint_toggle,
  //   work_area->storage_device_info.data_bulk_in_endpoint_toggle);

  amount_received = 0;
  result          = hw_data_in_transfer((uint8_t *)work_area->scsi_device_info.csw,
                               sizeof(_scsi_command_status_wrapper),
                               work_area->storage_device_info.max_packet_size,
                               work_area->storage_device_info.data_bulk_in_endpoint_id,
                               device_address,
                               &amount_received,
                               &work_area->storage_device_info.data_bulk_in_endpoint_toggle);

  if (result != CH_USB_INT_SUCCESS) {
    yprintf(10, "Err8 %d ", result);
    return result;
  }

  // 55 53 42 53
  // compare tag?
  if (amount_received != sizeof(_scsi_command_status_wrapper)) {
    yprintf(75, "E9 (%d)", amount_received);
    return 100;
  }

  if (work_area->scsi_device_info.csw.cbwstatus != 0) {
    yprintf(40, " cbwstatus(%d) ", work_area->scsi_device_info.csw.cbwstatus);
    return 99;
  }

  return CH_USB_INT_SUCCESS;
}

uint8_t scsi_inquiry(ch376_work_area *const work_area) {
  (void)work_area;
  uint8_t result;
  if ((result = do_scsi_cmd(work_area,
                            work_area->storage_device_info.device_address,
                            0,
                            sizeof(scsi_packet_inquiry),
                            0x24,
                            (uint8_t *)scsi_packet_inquiry,
                            work_area->scsi_device_info.buffer,
                            false)) != CH_USB_INT_SUCCESS) {
    return result;
  }

  memcpy(work_area->scsi_device_info.vendorid,
         work_area->scsi_device_info.buffer + 8,
         sizeof(work_area->scsi_device_info.vendorid));
  memcpy(work_area->scsi_device_info.productid,
         work_area->scsi_device_info.buffer + 16,
         sizeof(work_area->scsi_device_info.productid));
  memcpy(work_area->scsi_device_info.productrev, work_area->scsi_device_info.buffer + 32, 4); // todo is this right?

  return CH_USB_INT_SUCCESS;
}

bool scsi_test(ch376_work_area *const work_area) __z88dk_fastcall {
  return do_scsi_cmd(work_area,
                     work_area->storage_device_info.device_address,
                     0,
                     sizeof(_scsi_packet_test),
                     0,
                     (uint8_t *)scsi_packet_test,
                     work_area->scsi_device_info.buffer,
                     false) == CH_USB_INT_SUCCESS;
}

void scsi_request_sense(ch376_work_area *const work_area) __z88dk_fastcall {
  do_scsi_cmd(work_area,
              work_area->storage_device_info.device_address,
              0,
              sizeof(_scsi_packet_request_sense),
              18,
              (uint8_t *)scsi_packet_request_sense,
              work_area->scsi_device_info.buffer,
              false);
}

void wait_for_mounting(ch376_work_area *const work_area) __z88dk_fastcall {
  while (!scsi_test(work_area)) {
    scsi_request_sense(work_area);
  }

  work_area->usb_mounted = true;
  work_area->unknown     = true;
  work_area->dsk_changed = true;
}

uint8_t usb_host_init_old() {
  work_area *const p = get_work_area();
  printf("usb_host_init %p\r\n", p);

  initialise_work_area(p);

  ch376_reset();

  if (!ch376_probe()) {
    printf("CH376:           NOT PRESENT\r\n");
    return false;
  }

  p->ch376.present  = true;
  const uint8_t ver = ch376_get_firmware_version();
  printf("CH376:           PRESENT (VER %d)\r\n", ver);

  usb_host_bus_reset();
  delay(10);

  if (!fn_connect(p))
    return false;

  uint16_t amount_transferred;
  uint8_t  result;
  if ((result = scsi_max_luns(p, &amount_transferred)) != CH_USB_INT_SUCCESS) {
    printf("Err-scsi_max_luns %d\r\n", result);
    return false;
  }

  scsi_init(&p->ch376);

  if ((result = scsi_inquiry(&p->ch376)) != CH_USB_INT_SUCCESS) {
    printf("Err-scsi_inquiry %d\r\n", result);

    return false;
  }

  wait_for_mounting(&p->ch376);

  p->ch376.usb_present = true;

  return true;
}
