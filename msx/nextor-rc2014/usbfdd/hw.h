#ifndef __HW
#define __HW

#include "usb.h"
#include <stdlib.h>

extern uint8_t hw_control_transfer(const setup_packet *const cmd_packet,
                                   uint8_t *const            buffer,
                                   const uint8_t             device_address,
                                   const uint8_t             max_packet_size,
                                   uint16_t *const           amount_transferred);

extern uint8_t hw_get_description(const uint8_t device_address, device_descriptor *const buffer);

extern uint8_t hw_get_config_descriptor(config_descriptor *const buffer,
                                        const uint8_t            config_index,
                                        const uint8_t            max_packet_size,
                                        const uint8_t            buffer_size,
                                        const uint8_t            device_address,
                                        uint16_t *const          amount_transferred);

extern uint8_t hw_set_address(const uint8_t usb_address, const uint8_t packet_size);

extern uint8_t
usb_set_configuration(const uint8_t configuration_value, const uint8_t packet_size, const uint8_t device_address);

#endif
