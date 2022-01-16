#ifndef __ARGUMENTS
#define __ARGUMENTS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern uint8_t clock_speed_code;
extern const char* clock_speed;

void process_cli_arguments(const int argc, const char **argv);

#endif
