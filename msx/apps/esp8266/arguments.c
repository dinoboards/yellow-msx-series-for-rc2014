#include "arguments.h"
#include "print.h"
#include <stdbool.h>

uint8_t     subCommand = 0;
const char *pNewTimeZone;
const char *pNewSSID;
const char *pNewPassword;
uint16_t    baud_rate = 7 * 256 + 7; // 19200

const char *pFossilBaudRates[12] = {"75", "300", "600", "1200", "2400", "4800", "9600", "19200", "38400", "57600", "unknown", "115200"};

uint8_t abort_with_help() {
  print_str("Usage:  esp8266 <options> <subcommand>\r\n\r\n"
            "Utility to manage RC2014 Wifi Module\r\n\r\n"
            "  /b<rate>, /baud=<rate>\r\n"
            "    The desired tx/rx baud <rate>\r\n"
            "    eg 19200\r\n"
            "  hangup\r\n"
            "    Issue command to hangup remote\r\n"
            "    connection\r\n"
            "  time-sync\r\n"
            "    Sync the MSX RTC with\r\n"
            "    internet time\r\n"
            "  set-timezone <zone>\r\n"
            "    Set timezone\r\n"
            "  set-wifi <ssid> <password>\r\n"
            "    Set wifi credentials\r\n"
            "\r\n");
  exit(1);

  return 255;
}

uint8_t abort_with_invalid_options() {
  print_str("Invalid usage\r\n");
  return abort_with_help();
}

uint8_t arg_sub_command(const uint8_t i, const char **argv, const int argc) {
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

    if (strncasecmp(argv[i], "set-timezone", 6) == 0) {
      if (i + 1 >= argc)
        return abort_with_invalid_options();
      subCommand = SUB_COMMAND_SET_TIMEZONE;
      pNewTimeZone = argv[i + 1];
      return i + 2;
    }

    if (strncasecmp(argv[i], "set-wifi", 6) == 0) {
      if (i + 2 >= argc)
        return abort_with_invalid_options();
      subCommand = SUB_COMMAND_SET_WIFI;
      pNewSSID = argv[i + 1];
      pNewPassword = argv[i + 2];
      return i + 3;
    }

    // if (strncasecmp(argv[i], "wget", 4) == 0) {
    //   if (i + 2 >= argc)
    //     return abort_with_invalid_options();
    //   subCommand = SUB_COMMAND_SET_WIFI;
    //   pWgetUrl = argv[i + 1];
    //   return i + 2;
    // }

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

uint8_t arg_baud_rate(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  const char *arg_value;
  if (strncasecmp(arg_switch, "/baud", 5) == 0) {
    if (arg_switch[5] != '=') {
      print_str("Invalid baud switch - use /baud=<rate>\r\n");
      return abort_with_help();
    }

    arg_value = arg_switch + 6;
    goto process_baud_rate;
  } else if (strncmp(arg_switch, "/b", 2) == 0) {
    arg_value = arg_switch + 2;
    goto process_baud_rate;
  } else
    return i;

process_baud_rate:
  if (strcmp(arg_value, "75") == 0) {
    baud_rate = 0;
    return i + 1;
  }

  if (strcmp(arg_value, "300") == 0) {
    baud_rate = 1 * 256 + 1;
    return i + 1;
  }

  if (strcmp(arg_value, "600") == 0) {
    baud_rate = 2 * 256 + 2;
    return i + 1;
  }

  if (strcmp(arg_value, "1200") == 0) {
    baud_rate = 3 * 256 + 3;
    return i + 1;
  }

  if (strcmp(arg_value, "2400") == 0) {
    baud_rate = 4 * 256 + 4;
    return i + 1;
  }

  if (strcmp(arg_value, "4800") == 0) {
    baud_rate = 5 * 256 + 5;
    return i + 1;
  }

  if (strcmp(arg_value, "9600") == 0) {
    baud_rate = 6 * 256 + 6;
    return i + 1;
  }

  if (strcmp(arg_value, "19200") == 0) {
    baud_rate = 7 * 256 + 7;
    return i + 1;
  }

  if (strcmp(arg_value, "38400") == 0) {
    baud_rate = 8 * 256 + 8;
    return i + 1;
  }

  if (strcmp(arg_value, "57600") == 0) {
    baud_rate = 9 * 256 + 9;
    return i + 1;
  }

  if (strcmp(arg_value, "115200") == 0) {
    baud_rate = 11 * 256 + 11;
    return i + 1;
  }

  print_str("Invalid baud rate setting '");
  print_str(arg_value);
  print_str("'\r\n");
  exit(1);

  return 255;
}

bool isBlank(const char *str) {
  while (*str && *str == ' ')
    str++;

  return *str == 0;
}

void process_cli_arguments(const char **argv, const int argc) {
  for (uint8_t i = 1; i < argc;) {
    const uint8_t current_i = i;

    if (isBlank(argv[i])) {
      i++;
      continue;
    }

    i = arg_baud_rate(i, argv);
    if (current_i != i)
      continue;

    i = arg_help_msg(i, argv);
    if (current_i != i)
      continue;

    i = arg_sub_command(i, argv, argc);
    if (current_i != i)
      continue;

    abort_with_invalid_arg_msg(i, argv);
  }

  if (subCommand == 0)
    abort_with_missing_sub_command_msg();
}
