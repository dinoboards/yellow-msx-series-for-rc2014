#ifndef __ARGUMENT_H__
#define __ARGUMENT_H__

#include <stdint.h>

typedef enum { SET_COM = 1, SET_WIDTH = 2 } command_mode_t;

extern command_mode_t command_mode;

extern uint8_t process_cli_arguments(const int argc, char **argv);

#endif
