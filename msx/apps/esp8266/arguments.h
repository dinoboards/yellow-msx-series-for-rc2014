#ifndef __ARGUMENTS
#define __ARGUMENTS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUB_COMMAND_TIME_SYNC    1
#define SUB_COMMAND_HANGUP       2
#define SUB_COMMAND_SET_TIMEZONE 3

extern uint8_t     subCommand;
extern const char *pNewTimeZone;

void process_cli_arguments(const char **argv, const int argc);

#endif
