#include "arguments.h"
#include "print.h"

uint8_t subCommand = 0;

uint8_t abort_with_help() {
  print_str("Usage:  esp8266 <subcommand> [options]\r\n\r\n"
            "Utility to manage RC2014 Wifi Module\r\n\r\n"
            "  hangup\r\n"
            "    Issue command to hangup remote\r\n"
            "    connection\r\n"
            "  time-sync\r\n"
            "    Sync the MSX RTC with\r\n"
            "    internet time\r\n"
            "\r\n");
  exit(1);

  return 255;
}

uint8_t abort_with_invalid_options() {
  print_str("Invalid usage\r\n");
  return abort_with_help();
}

uint8_t arg_sub_command(const uint8_t i, const char **argv) {
  if (argv[i][0] != '/') {
    if (subCommand)
      return abort_with_invalid_options();

    if (strncasecmp(argv[i], "time-sync", 9) == 0) {
      subCommand = SUB_COMMAND_TIME_SYNC;
      return i + 1;
    }

    if (strncasecmp(argv[i], "hangup", 6) == 0) {
      subCommand = SUB_COMMAND_HANGUP;
      return i + 1;
    }

    return abort_with_invalid_options();
  }

  return i;
}

uint8_t arg_help_msg(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  if (strcmp(arg_switch, "/h") != 0 && strcasecmp(arg_switch, "/help") != 0)
    return i;

  return abort_with_help();
}

uint8_t abort_with_invalid_arg_msg(const uint8_t i, const char **argv) {
  print_str("Invalid command line option '");
  print_str((char *)argv[i]);
  print_str("'\r\n\r\n");
  return abort_with_help();
}

uint8_t abort_with_missing_sub_command_msg() {
  print_str("Missing subcommand argument.\r\n\r\n");
  return abort_with_help();
}

void process_cli_arguments(const char **argv, const int argc) {
  for (uint8_t i = 1; i < argc;) {
    const uint8_t current_i = i;

    i = arg_help_msg(i, argv);
    if (current_i != i)
      continue;

    i = arg_sub_command(i, argv);
    if (current_i != i)
      continue;

    abort_with_invalid_arg_msg(i, argv);
  }

  if (subCommand == 0)
    abort_with_missing_sub_command_msg();
}
