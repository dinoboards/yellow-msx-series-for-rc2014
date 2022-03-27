#ifndef __HW
#define __HW

#include "ch376.h"
#include "usb.h"
#include <stdlib.h>

extern usb_error hw_control_transfer(const setup_packet *const cmd_packet,
                                     uint8_t *const            buffer,
                                     const uint8_t             device_address,
                                     const uint8_t             max_packet_size,
                                     uint16_t *const           amount_transferred);

extern usb_error hw_get_description(const uint8_t device_address, device_descriptor *const buffer);

extern usb_error hw_get_config_descriptor(config_descriptor *const buffer,
                                          const uint8_t            config_index,
                                          const uint8_t            max_packet_size,
                                          const uint8_t            buffer_size,
                                          const uint8_t            device_address,
                                          uint16_t *const          amount_transferred);

extern usb_error hw_set_address(const uint8_t usb_address, const uint8_t packet_size);

extern usb_error
usb_set_configuration(const uint8_t configuration_value, const uint8_t packet_size, const uint8_t device_address);

usb_error hw_data_in_transfer(uint8_t *             buffer,
                              const uint16_t        buffer_size,
                              const uint8_t         device_address,
                              endpoint_param *const endpoint,
                              uint16_t *const       amount_received);

usb_error hw_data_out_transfer(const uint8_t *       buffer,
                               uint16_t              buffer_size,
                               const uint8_t         device_address,
                               endpoint_param *const endpoint);

#endif
