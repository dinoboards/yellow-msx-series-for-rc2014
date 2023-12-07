#include "argument.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

uint8_t redirect_stream;

void upcase_string(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = toupper((unsigned char)str[i]);
  }
}

const unsigned char *usage_message = "Change TTY.\r\n\n"
                                     "CTTY [STDIN|STDOUT] [DEVICE...]\r\n"
                                     "  STDIN   - redirect only STDIN\r\n"
                                     "  STDOUT  - redirect only STDOUT\r\n"
                                     "  DEVICE  - list of device names:\r\n"
                                     "             COMm CON LPT\r\n";

void show_help_message(void) { printf(usage_message); }

uint8_t process_cli_arguments(const int argc, char **argv) {
  argv;
  if (argc <= 1) {
    show_help_message();
    return 255;
  }

  redirect_stream = 0;

  for (int i = 1; i < argc; i++)
    upcase_string(argv[i]);

  for (uint8_t i = 1; i < argc; i++) {
    if (strcmp(argv[i], "STDIN") == 0) {
      if (redirect_stream) {
        show_help_message();
        return 255;
      }

      redirect_stream = REDIRECT_STDIN;
    }

    if (strcmp(argv[i], "STDOUT") == 0) {
      if (redirect_stream) {
        show_help_message();
        return 255;
      }
      redirect_stream = REDIRECT_STDOUT;
      return 0;
    }

    if (strcmp(argv[i], "CON") == 0)
      redirect_stream |= REDIRECT_CON;
    else if (strcmp(argv[i], "COM1") == 0)
      redirect_stream |= REDIRECT_COM1;
    else if (strcmp(argv[i], "COM2") == 0)
      redirect_stream |= REDIRECT_COM2;
    else if (strcmp(argv[i], "COM3") == 0)
      redirect_stream |= REDIRECT_COM3;
    else if (strcmp(argv[i], "COM4") == 0)
      redirect_stream |= REDIRECT_COM4;
    else if (strcmp(argv[i], "LPT1") == 0)
      redirect_stream |= REDIRECT_LPT;
  }

  if ((redirect_stream & (REDIRECT_STDIN | REDIRECT_STDOUT)) == 0)
    redirect_stream |= REDIRECT_STDIN | REDIRECT_STDOUT;

  return 0;
}
