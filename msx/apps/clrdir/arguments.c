#include "arguments.h"

char drive_letter;

const unsigned char *usage = "Usage:  clrdir [drive-leter] [options]\r\n\n"
                             "erase all files from a floppy\r\n\n"
                             "/h       Display help message\r\n\n"
                             "/f:size  Floppy size (1440)\r\n\n"
                             "<drive-letter>\r\n"
                             "         Floppy drive to be erased\r\n";

uint8_t abort_with_help(void) {
  printf(usage);
  exit(1);

  return 255;
}

uint8_t arg_floppy_size(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  if (arg_switch[0] != '/' && arg_switch[1] != 'f')
    return i;

  if (arg_switch[2] != ':') {
    printf("Invalid /f option\r\n");
    return abort_with_help();
  }

  if (strcmp(arg_switch, "/f:1440") == 0) {
    // floppy_size = 1440;
    return i + 1;
  }

  printf("Only 1440 (1.44Mb) floppys supported\r\n");
  return abort_with_help();
}

uint8_t arg_help_msg(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  if (strcmp(arg_switch, "/h") != 0 && strcasecmp(arg_switch, "/help") != 0)
    return i;

  return abort_with_help();
}

uint8_t abort_with_invalid_arg_msg(const uint8_t i, const char **argv) {
  printf("Invalid command line option '");
  printf((char *)argv[i]);
  printf("'\r\n\r\n");
  return abort_with_help();
}

uint8_t arg_drive_letter(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];

  if (arg_switch[0] == '/')
    return i;

  if (strlen(arg_switch) != 2 || arg_switch[1] != ':')
    goto invalid;

  if ((arg_switch[0] >= 'A' && arg_switch[0] <= 'Z')) {
    drive_letter = arg_switch[0];
    return i + 1;
  }

  if ((arg_switch[0] >= 'a' && arg_switch[0] <= 'z')) {
    drive_letter = arg_switch[0] - ('a' - 'A');
    return i + 1;
  }

invalid:
  printf("Invalid drive letter (A: to Z:)\r\n");
  return abort_with_help();
}

void process_cli_arguments(const int argc, const char **argv) {
  drive_letter = 0;

  for (uint8_t i = 1; i < argc;) {
    const uint8_t current_i = i;

    i = arg_help_msg(i, argv);
    if (current_i != i)
      continue;

    i = arg_floppy_size(i, argv);
    if (current_i != i)
      continue;

    i = arg_drive_letter(i, argv);
    if (current_i != i)
      continue;

    abort_with_invalid_arg_msg(i, argv);
  }

  if (drive_letter == 0) {
    printf("Missing drive_letter\r\n");
    abort_with_help();
  }
}
