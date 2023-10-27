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
  cmd_floppy_report_drive,
  cmd_floppy_dump,
  cmd_printer_check,
  cmd_ftdi_check,
} subcommands;

extern const char *report_file_name;
extern char        floppy_drive_letter;
extern bool        quick_format;
extern subcommands subcommand;

void process_cli_arguments(const int argc, const char **argv) __sdcccall(1);

#endif
