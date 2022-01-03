#include "arguments.h"
#include "print.h"
#include <stdbool.h>

const char *flash_file_name;

uint8_t abort_with_help() {
  printf("Usage:  muflash <filename>\r\n\r\n"
         "Flash the onboard ROM of the MSX MUSIC Module\r\n\r\n"
         "  filename\r\n"
         "    The file name of image to flash\r\n"
         "\r\n");
  exit(1);

  return 255;
}

uint8_t arg_help_msg(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  if (strcmp(arg_switch, "/h") != 0 && strcasecmp(arg_switch, "/help") != 0)
    return i;

  return abort_with_help();
}

uint8_t arg_file_name(const uint8_t i, const char **argv) {
  if (argv[i][0] != '/') {
    if (flash_file_name)
      return abort_with_help();
    flash_file_name = argv[i];
    return i + 1;
  }

  return i;
}

uint8_t abort_with_invalid_arg_msg(const uint8_t i, const char **argv) {
  printf("Invalid command line option '");
  printf((char *)argv[i]);
  printf("'\r\n\r\n");
  return abort_with_help();
}

uint8_t abort_with_missing_file_name_msg() {
  printf("Missing filename.\r\n\r\n");
  return abort_with_help();
}

void process_cli_arguments(const int argc, const char **argv) {
  for (uint8_t i = 1; i < argc;) {
    const uint8_t current_i = i;

    i = arg_help_msg(i, argv);
    if (current_i != i)
      continue;

    i = arg_file_name(i, argv);
    if (current_i != i)
      continue;

    abort_with_invalid_arg_msg(i, argv);
  }

  if (flash_file_name == NULL)
    abort_with_missing_file_name_msg();
}
