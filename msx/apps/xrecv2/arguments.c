#include "arguments.h"
#include "print.h"

const char *pFileName = NULL;
uint16_t    baud_rate = 7 * 256 + 7; // 19200

#define FOSSIL_BAUD_RATE_75 1 * 256 + 1

const char *pFossilBaudRates[12] = {"75", "300", "600", "1200", "2400", "4800", "9600", "19200", "38400", "57600", "unknown", "115200"};

uint8_t abort_with_help() {
  print_str("Usage:  xrecv2 <filename> [options]\r\n\r\nReceive a file using xmodem protocol\r\nusing the active fossil driver\r\n\r\n  /b<rate>, /baud=<rate>\r\n    The desired tx/rx baud <rate>\r\n    eg 19200\r\n");
  exit(1);

  return 255;
}

uint8_t abort_with_invalid_options() {
  print_str("Invalid usage\r\n");
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

uint8_t arg_file_name(const uint8_t i, const char **argv) {
  if (argv[i][0] != '/') {
    if (pFileName)
      return abort_with_invalid_options();
    pFileName = argv[i];
    return i + 1;
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

uint8_t abort_with_missing_file_name_msg() {
  print_str("Missing filename\r\n\r\n");
  return abort_with_help();
}

void process_cli_arguments(char **argv, int argc) {
  for (uint8_t i = 0; i < argc;) {
    const uint8_t current_i = i;
    i = arg_baud_rate(i, argv);
    if (current_i != i)
      continue;

    i = arg_help_msg(i, argv);
    if (current_i != i)
      continue;

    i = arg_file_name(i, argv);
    if (current_i != i)
      continue;

    abort_with_invalid_arg_msg(i, argv);
  }

  if (pFileName == NULL)
    abort_with_missing_file_name_msg();
}
