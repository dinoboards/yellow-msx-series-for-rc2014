#ifndef __ARGUMENTS
#define __ARGUMENTS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUB_COMMAND_TIME_SYNC 1
#define SUB_COMMAND_HANGUP    2

extern uint8_t subCommand;

void process_cli_arguments(const char **argv, const int argc);

#endif
