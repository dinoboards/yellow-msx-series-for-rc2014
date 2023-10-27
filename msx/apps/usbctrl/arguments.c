#include "arguments.h"
#include <stdint.h>
#include <string.h>

const char *report_file_name;
char        floppy_drive_letter;
subcommands subcommand;
bool        quick_format;
uint32_t    baud_rate;

const unsigned char *usage = "Usage: usbctrl <subcommand> ... <options>\r\n\n"
                             "inspected and managed connected\r\n"
                             "USB devices\r\n\n"
                             "/h       Display help message\r\n\n"
                             " report <filename>\r\n"
                             "   report all connected devices to a file\r\n"
                             " floppy help \r\n"
                             "  display floppy commands\r\n"
                             " printer help\r\n"
                             "  display printer commands\r\n"
                             " ftdi help\r\n"
                             "  display ftdi commands\r\n";

const unsigned char *floppy_help = "Usage: usbctrl floppy ...\r\n\n"
                                   " floppy report\r\n"
                                   "   identify all connected floppy drives\r\n"
                                   " floppy report <drive-letter>:\r\n"
                                   "   describe floppy drive/disk\r\n"
                                   " floppy format <drive-letter>: [/q]\r\n"
                                   "   initiate ufi format command\r\n"
                                   "   /q do a quick format\r\n"
                                   " floppy check <drive-letter>:\r\n"
                                   "   write and verify all sectors\r\n";

const unsigned char *printer_help = "Usage: usbctrl printer ....\r\n\n"
                                    " printer check\r\n"
                                    "   send sample text to printer\r\n";

const unsigned char *ftdi_help = "Usage: usbctrl ftdi\r\n\n"
                                 " ftdi check\r\n"
                                 "   check ftdi sio\r\n";

#define abort_with_help() abort_with_help_msg(usage);

arg_search abort_with_help_msg(const unsigned char *const msg) {
  printf(msg);
  exit(1);

  return arg_search_abort;
}

arg_search check_report_commands(const int argc, const char **const argv) __sdcccall(1) {
  if (argc < 2)
    return arg_search_continue;

  const char *arg_subcommand = argv[1];
  if (strncmp(arg_subcommand, "report", 7) != 0)
    return arg_search_continue;

  if (argc != 3)
    return abort_with_help();

  const char *arg_filename = argv[2];

  subcommand       = cmd_report_all_devices;
  report_file_name = argv[2];
  return arg_search_abort;
}

bool arg_is_help_command(const unsigned char *const command, const char **argv) __sdcccall(1) {

  const char *arg_subcommand = argv[1];
  if (strncmp(arg_subcommand, command, strlen(command)) != 0)
    return false;

  const char *action = argv[2];
  if (strncmp(action, "help", 5) == 0)
    return true;

  return false;
}

arg_search arg_floppy_help(const char **const argv) __sdcccall(1) {
  if (arg_is_help_command("floppy", argv))
    return abort_with_help_msg(floppy_help);

  return arg_search_continue;
}

arg_search arg_report_floppies(const char **const argv, const int argc) __sdcccall(1) {
  if (argc < 3)
    return arg_search_continue;

  const char *floppy_action = argv[2];
  if (strncmp(floppy_action, "report", 7) != 0)
    return arg_search_continue;

  if (argc == 3) {
    subcommand = cmd_floppy_report;
    return arg_search_abort;
  }

  if (argc == 4) {
    subcommand                         = cmd_floppy_report_drive;
    const char *const arg_drive_letter = argv[3];

    if (strlen(arg_drive_letter) != 2 || arg_drive_letter[1] != ':')
      goto invalid;

    if ((arg_drive_letter[0] >= 'A' && arg_drive_letter[0] <= 'Z')) {
      floppy_drive_letter = arg_drive_letter[0];
      return arg_search_abort;
    }

    if ((arg_drive_letter[0] >= 'a' && arg_drive_letter[0] <= 'z')) {
      floppy_drive_letter = arg_drive_letter[0] - ('a' - 'A');
      return arg_search_abort;
    }

  invalid:
    printf("Invalid drive letter (A: to Z:)\r\n");
    return abort_with_help();
  }

  return abort_with_help();
}

arg_search arg_format_floppy(const char **const argv, const int argc) __sdcccall(1) {
  if (argc < 3)
    return arg_search_continue;

  const char *floppy_action = argv[2];
  if (strncmp(floppy_action, "format", 7) != 0)
    return arg_search_continue;

  if (argc != 4)
    return abort_with_help();

  subcommand                         = cmd_floppy_format;
  const char *const arg_drive_letter = argv[3];

  if (strlen(arg_drive_letter) != 2 || arg_drive_letter[1] != ':')
    goto invalid;

  if ((arg_drive_letter[0] >= 'A' && arg_drive_letter[0] <= 'Z')) {
    floppy_drive_letter = arg_drive_letter[0];
    return arg_search_abort;
  }

  if ((arg_drive_letter[0] >= 'a' && arg_drive_letter[0] <= 'z')) {
    floppy_drive_letter = arg_drive_letter[0] - ('a' - 'A');
    return arg_search_abort;
  }

invalid:
  printf("Invalid drive letter (A: to Z:)\r\n");
  return abort_with_help();
}

arg_search arg_command_floppy_check(const char **const argv, const int argc) __sdcccall(1) {
  if (argc < 3)
    return arg_search_continue;

  const char *floppy_action = argv[2];
  if (strncmp(floppy_action, "check", 6) != 0)
    return arg_search_continue;

  if (argc != 4)
    return abort_with_help();

  subcommand                         = cmd_floppy_check;
  const char *const arg_drive_letter = argv[3];

  if (strlen(arg_drive_letter) != 2 || arg_drive_letter[1] != ':')
    goto invalid;

  if ((arg_drive_letter[0] >= 'A' && arg_drive_letter[0] <= 'Z')) {
    floppy_drive_letter = arg_drive_letter[0];
    return arg_search_abort;
  }

  if ((arg_drive_letter[0] >= 'a' && arg_drive_letter[0] <= 'z')) {
    floppy_drive_letter = arg_drive_letter[0] - ('a' - 'A');
    return arg_search_abort;
  }

invalid:
  printf("Invalid drive letter (A: to Z:)\r\n");
  return abort_with_help();
}

arg_search arg_printer_help(const char **const argv) __sdcccall(1) {
  if (arg_is_help_command("printer", argv))
    return abort_with_help_msg(printer_help);

  return arg_search_continue;
}

arg_search arg_command_printer_check(const char **const argv, const int argc) __sdcccall(1) {
  if (argc < 3)
    return arg_search_continue;

  const char *floppy_action = argv[2];
  if (strncmp(floppy_action, "check", 6) != 0)
    return arg_search_continue;

  if (argc != 3)
    return abort_with_help();

  subcommand = cmd_printer_check;
  return arg_search_abort;
}

arg_search arg_ftdi_help(const char **const argv) __sdcccall(1) {
  if (arg_is_help_command("ftdi", argv))
    return abort_with_help_msg(ftdi_help);

  return arg_search_continue;
}

arg_search arg_command_ftdi_check(const char **const argv, const int argc) __sdcccall(1) {
  if (argc < 3)
    return arg_search_continue;

  const char *floppy_action = argv[2];
  if (strncmp(floppy_action, "check", 6) != 0)
    return arg_search_continue;

  if (argc != 3)
    return abort_with_help();

  subcommand = cmd_ftdi_check;
  return arg_search_abort;
}

arg_search arg_command_floppy_dump(const char **const argv, const int argc) __sdcccall(1) {
  if (argc < 3)
    return arg_search_continue;

  const char *floppy_action = argv[2];
  if (strncmp(floppy_action, "dump", 5) != 0)
    return arg_search_continue;

  if (argc != 4)
    return abort_with_help();

  subcommand                         = cmd_floppy_dump;
  const char *const arg_drive_letter = argv[3];

  if (strlen(arg_drive_letter) != 2 || arg_drive_letter[1] != ':')
    goto invalid;

  if ((arg_drive_letter[0] >= 'A' && arg_drive_letter[0] <= 'Z')) {
    floppy_drive_letter = arg_drive_letter[0];
    return arg_search_abort;
  }

  if ((arg_drive_letter[0] >= 'a' && arg_drive_letter[0] <= 'z')) {
    floppy_drive_letter = arg_drive_letter[0] - ('a' - 'A');
    return arg_search_abort;
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

arg_search check_floppy_commands(const int argc, const char **const argv) __sdcccall(1) {
  arg_search i;

  i = arg_floppy_help(argv);
  if (i == arg_search_abort)
    return i;

  i = arg_report_floppies(argv, argc);
  if (i == arg_search_abort)
    return i;

  i = arg_format_floppy(argv, argc);
  if (i == arg_search_abort)
    return i;

  i = arg_command_floppy_check(argv, argc);
  if (i == arg_search_abort)
    return i;

  i = arg_command_floppy_dump(argv, argc);
  if (i == arg_search_abort)
    return i;

  return arg_search_continue;
}

arg_search check_printer_commands(const int argc, const char **const argv) __sdcccall(1) {
  arg_search i;

  i = arg_printer_help(argv);
  if (i == arg_search_abort)
    return i;

  i = arg_command_printer_check(argv, argc);
  if (i == arg_search_abort)
    return i;

  return arg_search_continue;
}

arg_search check_ftdi_commands(const int argc, const char **const argv) __sdcccall(1) {
  arg_search i;

  i = arg_ftdi_help(argv);
  if (i == arg_search_abort)
    return i;

  i = arg_command_ftdi_check(argv, argc);
  if (i == arg_search_abort)
    return i;

  return arg_search_continue;
}

arg_search process_cli_arguments(int argc, const char **const argv) __sdcccall(1) {
  report_file_name = NULL;
  subcommand       = cmd_none;
  quick_format     = false;
  baud_rate        = 4800;

  for (uint8_t i = argc - 1; i > 1; i++) {
    arg_help_msg(i, argv);

    if (argv[i][0] != '/')
      break;

    if (strcmp(argv[i], "/q") == 0) {
      quick_format = true;
      argc--;
      continue;
    }

    if (strncmp(argv[i], "/b=", 3) == 0) {
      baud_rate = atoi(argv[i] + 3);
      argc--;
      continue;
    }

    printf("Invalid option %s\r\n", argv[i]);
    return abort_with_help();
  }

  main_commands main_command = cmd_unknown;

  if (argc > 1) {
    const char *arg_subcommand = argv[1];
    if (strcmp(arg_subcommand, "report") == 0) {
      if (check_report_commands(argc, argv) == arg_search_abort)
        return arg_search_abort;
    } else if (strcmp(arg_subcommand, "floppy") == 0) {
      if (check_floppy_commands(argc, argv) == arg_search_abort)
        return arg_search_abort;
    } else if (strcmp(arg_subcommand, "printer") == 0) {
      if (check_printer_commands(argc, argv) == arg_search_abort)
        return arg_search_abort;
    } else if (strcmp(arg_subcommand, "ftdi") == 0) {
      if (check_ftdi_commands(argc, argv) == arg_search_abort)
        return arg_search_abort;
    }
  }

  printf("Missing sub command\r\n");
  return abort_with_help();
}
