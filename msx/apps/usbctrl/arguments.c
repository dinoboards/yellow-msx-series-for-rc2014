#include "arguments.h"
#include <stdint.h>
#include <string.h>

const char *report_file_name;
subcommands subcommand;

const unsigned char *usage = "Usage: usbctrl <options> <subcommand>\r\n\n"
                             "inspected and managed connected\r\nUSB devices\r\n\n"
                             "/h       Display help message\r\n\n"
                             " report <filename>\r\n"
                             " report all connected devices to the file\r\n\n"
                             " report-floppy\r\n"
                             " identify connected floppy desvices\r\n\n";

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
  if (strncmp(arg_subcommand, "report-floppy", 14) != 0)
    return i;

  if (argc != 2)
    return abort_with_help();

  subcommand = cmd_report_floppies;

  return i + 1;
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

    abort_with_invalid_arg_msg(i, argv);
  }

  if (subcommand == cmd_none) {
    printf("Missing sub command\r\nExpected report or report-floppy\r\n");
    abort_with_help();
  }
}
