#ifndef __ARGUMENTS
#define __ARGUMENTS

/* must include stdbool before stdio, as stdio include msx.h which will define its own bool if none defined */
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  none,
  report_to_file,
} subcommands;

extern const char *report_file_name;
extern subcommands subcommand;

void process_cli_arguments(const int argc, const char **argv) __sdcccall(1);

#endif
