#include "argument.h"
#include "argument_com.h"
#include "argument_width.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

command_mode_t command_mode;

void upcase_string(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = toupper((unsigned char)str[i]);
  }
}

const unsigned char *usage_message = "Configures system devices.\r\n\n"
                                     "Serial port:   MODE COMm[:] [BAUD=b] [PARITY=p] [DATA=d] [STOP=s]\r\n"
                                     "Device Status: MODE [device] [/STATUS]\r\n"
                                     "Redirect:      MODE [device]=[device]\r\n"
                                     "Display mode:  MODE CON[:] [COLS=c] [LINES=n]\r\n"
                                     "Display width: MODE <cols>\r\n";

void show_help_message(void) { printf(usage_message); }

uint8_t process_cli_arguments(const int argc, char **argv) {
  if (argc < 2) {
    show_help_message();
    return 255;
  }

  for (int i = 1; i < argc; i++)
    upcase_string(argv[i]);

  if (strncmp(argv[1], "COM", 3) == 0)
    return process_port(argc, argv);

  if (process_con_width(argc, argv) == 0)
    return 0;

  show_help_message();
  return 255;
}
