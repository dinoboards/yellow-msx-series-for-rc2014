#include "arguments.h"
#include <stdint.h>
#include <string.h>

const char             *report_file_name;
char                    floppy_drive_letter;
subcommands             subcommand;
device_config_interface device_interface;

const unsigned char *usage = "Usage: usbctrl <options> <subcommand>\r\n\n"
                             "inspected and managed connected\r\nUSB devices\r\n\n"
                             "/h       Display help message\r\n\n"
                             " report <filename>\r\n"
                             "   report all connected devices to the file\r\n"
                             " floppy report\r\n"
                             "   identify connected floppy devices\r\n"
                             " floppy find <drive-letter>:\r\n"
                             "   find the usb device id\r\n"
                             " floppy format <address> <config> <interface>\r\n"
                             "   initiate ufi format command\r\n"
                             " floppy check <address> <config> <interface>\r\n"
                             "   write and verify all sectors\r\n";

uint8_t abort_with_help(void) {
  printf(usage);
  exit(1);

  return 255;
}

uint8_t arg_report_to_file(const uint8_t i, const char **argv, const int argc) __sdcccall(1) {
  if (i != 1)
    return i;

  const char *arg_subcommand = argv[1];
  if (strncmp(arg_subcommand, "report", 7) != 0)
    return i;

  if (argc != 3)
    return abort_with_help();

  const char *arg_filename = argv[2];

  subcommand       = cmd_report_to_file;
  report_file_name = argv[2];
  return i + 2;
}

uint8_t arg_report_floppies(const uint8_t i, const char **argv, const int argc) __sdcccall(1) {
  if (i != 1)
    return i;

  const char *arg_subcommand = argv[1];
  if (strncmp(arg_subcommand, "floppy", 7) != 0)
    return i;

  const char *floppy_action = argv[2];
  if (strncmp(floppy_action, "report", 7) != 0)
    return i;

  if (argc != 3)
    return abort_with_help();

  subcommand = cmd_report_floppies;

  return i + 2;
}

uint8_t arg_format_floppy(const uint8_t i, const char **argv, const int argc) __sdcccall(1) {
  if (i != 1)
    return i;

  const char *arg_subcommand = argv[1];
  if (strncmp(arg_subcommand, "floppy", 7) != 0)
    return i;

  const char *floppy_action = argv[2];
  if (strncmp(floppy_action, "format", 7) != 0)
    return i;

  if (argc != 6)
    return abort_with_help();

  subcommand                       = cmd_format_floppy;
  device_interface.address         = atoi(argv[3]);
  device_interface.config_index    = atoi(argv[4]);
  device_interface.interface_index = atoi(argv[5]);
  return i + 5;
}

uint8_t arg_format_find(const uint8_t i, const char **argv, const int argc) __sdcccall(1) {
  if (i != 1)
    return i;

  const char *arg_subcommand = argv[1];
  if (strncmp(arg_subcommand, "floppy", 7) != 0)
    return i;

  const char *floppy_action = argv[2];
  if (strncmp(floppy_action, "find", 5) != 0)
    return i;

  if (argc != 4)
    return abort_with_help();

  subcommand                         = cmd_floppy_find;
  const char *const arg_drive_letter = argv[3];

  if (strlen(arg_drive_letter) != 2 || arg_drive_letter[1] != ':')
    goto invalid;

  if ((arg_drive_letter[0] >= 'A' && arg_drive_letter[0] <= 'Z')) {
    floppy_drive_letter = arg_drive_letter[0];
    return i + 4;
  }

  if ((arg_drive_letter[0] >= 'a' && arg_drive_letter[0] <= 'z')) {
    floppy_drive_letter = arg_drive_letter[0] - ('a' - 'A');
    return i + 4;
  }

invalid:
  printf("Invalid drive letter (A: to Z:)\r\n");
  return abort_with_help();
}

uint8_t arg_check_floppy_sectors(const uint8_t i, const char **argv, const int argc) __sdcccall(1) {
  if (i != 1)
    return i;

  const char *arg_subcommand = argv[1];
  if (strncmp(arg_subcommand, "floppy", 7) != 0)
    return i;

  const char *floppy_action = argv[2];
  if (strncmp(floppy_action, "check", 6) != 0)
    return i;

  if (argc != 6)
    return abort_with_help();

  subcommand                       = cmd_check_floppy_sectors;
  device_interface.address         = atoi(argv[3]);
  device_interface.config_index    = atoi(argv[4]);
  device_interface.interface_index = atoi(argv[5]);
  return i + 5;
}

uint8_t arg_help_msg(const uint8_t i, const char **argv) __sdcccall(1) {
  const char *arg_switch = argv[i];
  if (strcmp(arg_switch, "/h") != 0 && strcasecmp(arg_switch, "/help") != 0)
    return i;

  return abort_with_help();
}

uint8_t abort_with_invalid_arg_msg(const uint8_t i, const char **argv) __sdcccall(1) {

  printf("Invalid command line '");
  printf((char *)argv[i]);
  printf("'\r\n\r\n");
  return abort_with_help();
}

void process_cli_arguments(const int argc, const char **argv) __sdcccall(1) {
  report_file_name = NULL;
  subcommand       = cmd_none;

  for (uint8_t i = 1; i < argc;) {
    const uint8_t current_i = i;

    i = arg_help_msg(i, argv);
    if (current_i != i)
      continue;

    i = arg_report_to_file(i, argv, argc);
    if (current_i != i)
      continue;

    i = arg_report_floppies(i, argv, argc);
    if (current_i != i)
      continue;

    i = arg_format_floppy(i, argv, argc);
    if (current_i != i)
      continue;

    i = arg_check_floppy_sectors(i, argv, argc);
    if (current_i != i)
      continue;

    i = arg_format_find(i, argv, argc);
    if (current_i != i)
      continue;

    abort_with_invalid_arg_msg(i, argv);
  }

  if (subcommand == cmd_none) {
    printf("Missing sub command\r\nExpected report or report-floppy\r\n");
    abort_with_help();
  }
}
