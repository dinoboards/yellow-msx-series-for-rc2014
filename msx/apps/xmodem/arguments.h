#ifndef __ARGUMENTS
#define __ARGUMENTS

#include <stdbool.h>
#include <stdio.h>

extern const char *pFileName;
extern uint8_t     receive_file;
extern uint8_t     send_file;

extern void process_cli_arguments(const uint16_t argc, char **const argv);

#endif
