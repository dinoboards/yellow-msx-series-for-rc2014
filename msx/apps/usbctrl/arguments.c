#include "arguments.h"
#include <stdint.h>
#include <string.h>

const char *report_file_name;
char        floppy_drive_letter;
subcommands subcommand;
bool        quick_format;

const unsigned char *usage = "Usage: usbctrl <options> <subcommand>\r\n\n"
                             "inspected and managed connected\r\n"
                             "USB devices\r\n\n"
                             "/h       Display help message\r\n\n"
                             " report <filename>\r\n"
                             "   report all connected devices to a file\r\n"
                             " floppy report\r\n"
                             "   identify all connected floppy drives\r\n"
                             " floppy report <drive-letter>:\r\n"
                             "   describe floppy drive/disk\r\n"
                             " floppy format <drive-letter>: [/q]\r\n"
                             "   initiate ufi format command\r\n"
                             "   \\q do a quick format\r\n"
                             " floppy check <drive-letter>:\r\n"
                             "   write and verify all sectors\r\n"
                             " printer check\r\n"
                             "   send sample text to printer\r\n"
                             " ftdi check\r\n"
                             "   check ftdi sio\r\n";

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

  subcommand       = cmd_report_all_devices;
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

  if (argc == 3) {
    subcommand = cmd_floppy_report;
    return i + 2;
  }

  if (argc == 4) {
    subcommand                         = cmd_floppy_report_drive;
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

  return abort_with_help();
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

  if (argc != 4)
    return abort_with_help();

  subcommand                         = cmd_floppy_format;
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

uint8_t arg_command_floppy_check(const uint8_t i, const char **argv, const int argc) __sdcccall(1) {
  if (i != 1)
    return i;

  const char *arg_subcommand = argv[1];
  if (strncmp(arg_subcommand, "floppy", 7) != 0)
    return i;

  const char *floppy_action = argv[2];
  if (strncmp(floppy_action, "check", 6) != 0)
    return i;

  if (argc != 4)
    return abort_with_help();

  subcommand                         = cmd_floppy_check;
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

uint8_t arg_command_printer_check(const uint8_t i, const char **argv, const int argc) __sdcccall(1) {
  if (i != 1)
    return i;

  const char *arg_subcommand = argv[1];
  if (strncmp(arg_subcommand, "printer", 8) != 0)
    return i;

  const char *floppy_action = argv[2];
  if (strncmp(floppy_action, "check", 6) != 0)
    return i;

  if (argc != 3)
    return abort_with_help();

  subcommand = cmd_printer_check;
  return i + 3;
}

uint8_t arg_command_ftdi_check(const uint8_t i, const char **argv, const int argc) __sdcccall(1) {
  if (i != 1)
    return i;

  const char *arg_subcommand = argv[1];
  if (strncmp(arg_subcommand, "ftdi", 5) != 0)
    return i;

  const char *floppy_action = argv[2];
  if (strncmp(floppy_action, "check", 6) != 0)
    return i;

  if (argc != 3)
    return abort_with_help();

  subcommand = cmd_ftdi_check;
  return i + 3;
}

uint8_t arg_command_floppy_dump(const uint8_t i, const char **argv, const int argc) __sdcccall(1) {
  if (i != 1)
    return i;

  const char *arg_subcommand = argv[1];
  if (strncmp(arg_subcommand, "floppy", 7) != 0)
    return i;

  const char *floppy_action = argv[2];
  if (strncmp(floppy_action, "dump", 5) != 0)
    return i;

  if (argc != 4)
    return abort_with_help();

  subcommand                         = cmd_floppy_dump;
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

void process_cli_arguments(int argc, const char **argv) __sdcccall(1) {
  report_file_name = NULL;
  subcommand       = cmd_none;
  quick_format     = false;

  for (uint8_t i = argc - 1; i > 1; i++) {
    arg_help_msg(i, argv);

    if (argv[i][0] != '/')
      break;

    if (strcmp(argv[i], "/q") == 0) {
      quick_format = true;
      argc--;
      continue;
    }

    printf("Invalid option %s\r\n", argv[i]);
    abort_with_help();
  }

  for (uint8_t i = 1; i < argc;) {
    const uint8_t current_i = i;

    i = arg_report_to_file(i, argv, argc);
    if (current_i != i)
      continue;

    i = arg_report_floppies(i, argv, argc);
    if (current_i != i)
      continue;

    i = arg_format_floppy(i, argv, argc);
    if (current_i != i)
      continue;

    i = arg_command_floppy_check(i, argv, argc);
    if (current_i != i)
      continue;

    i = arg_command_floppy_dump(i, argv, argc);
    if (current_i != i)
      continue;

    i = arg_command_printer_check(i, argv, argc);
    if (current_i != i)
      continue;

    i = arg_command_ftdi_check(i, argv, argc);
    if (current_i != i)
      continue;

    abort_with_invalid_arg_msg(i, argv);
  }

  if (subcommand == cmd_none) {
    printf("Missing sub command\r\nExpected report or report-floppy\r\n");
    abort_with_help();
  }
}
