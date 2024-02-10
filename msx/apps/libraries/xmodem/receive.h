#ifndef _XMODEM_RECEIVE_H_
#define _XMODEM_RECEIVE_H_

#include <stdint.h>

extern uint8_t port_number;

#define PACKET_SIZE (DATA_SIZE + 5)

extern uint8_t xmodem_receive(const char *p_file_name, uint32_t *const retrieved_bytes, const uint8_t expect_info_packet);

#endif
