#ifndef __ARGUMENT_H__
#define __ARGUMENT_H__

#include <stdint.h>

extern uint8_t apply_to_stdin;
extern uint8_t apply_to_stdout;
extern uint8_t activate_com_port;
extern uint8_t activate_con;

extern uint8_t process_cli_arguments(const int argc, char **argv);

#endif
