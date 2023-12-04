#ifndef _ARGUMENT_COM_H_
#define _ARGUMENT_COM_H_

extern uint32_t baud_rate;
extern uint16_t parity; // one of SERIAL_PARITY_NONE, SERIAL_PARITY_ODD, SERIAL_PARITY_EVEN, SERIAL_PARITY_MARK, SERIAL_PARITY_SPACE
extern uint16_t stop_bit_count; // one of SERIAL_STOPBITS_1, SERIAL_STOPBITS_15, SERIAL_STOPBITS_2
extern uint8_t  port_number;
extern uint8_t  data_bits;

extern uint8_t process_port(const int argc, char **argv);

#endif
