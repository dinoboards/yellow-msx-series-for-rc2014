#ifndef __ARGUMENTS
#define __ARGUMENTS

/* must include stdbool before stdio, as stdio include msx.h which will define its own bool if none defined */
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  uint8_t address;
  uint8_t config_index;
  uint8_t interface_index;
} device_config_interface;

typedef enum {
  cmd_none,
  cmd_report_all_devices,
  cmd_floppy_report,
  cmd_floppy_format,
  cmd_floppy_check,
  cmd_floppy_find
} subcommands;

extern const char *report_file_name;
// extern device_config_interface device_interface;
extern char        floppy_drive_letter;
extern subcommands subcommand;

void process_cli_arguments(const int argc, const char **argv) __sdcccall(1);

#endif
