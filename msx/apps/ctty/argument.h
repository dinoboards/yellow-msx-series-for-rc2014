#ifndef __ARGUMENT_H__
#define __ARGUMENT_H__

#include <stdint.h>

#define REDIRECT_STDIN  1
#define REDIRECT_STDOUT 2

#define REDIRECT_CON  4
#define REDIRECT_COM1 8
#define REDIRECT_COM2 16
#define REDIRECT_COM3 32
#define REDIRECT_COM4 64
#define REDIRECT_LPT  128

extern uint8_t redirect_stream;

extern uint8_t process_cli_arguments(const int argc, char **argv);

#endif
