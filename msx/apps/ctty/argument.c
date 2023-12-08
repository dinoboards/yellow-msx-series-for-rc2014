#include "argument.h"
#include <ctype.h>
#include <extbio/serial.h>
#include <stdio.h>
#include <string.h>

uint8_t apply_to_stdin;
uint8_t apply_to_stdout;
uint8_t activate_com_port;
uint8_t activate_con;

void upcase_string(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = toupper((unsigned char)str[i]);
  }
}

/*  Examples */
/*

CTTY OUTPUT COM1 CON ; redirect all output to COM1 and CRT
CTTY INPUT COM1 CON ; redirect all input to both COM1 and CON
CTTY COM1 CON ; redirect all input and output to both COM1 and CON
CTTY CON ; redirect all input and output to CON only
CTTY NULL; disable all i/o

*/
const unsigned char *error_usage_message = "CTTY [STDIN|STDOUT] [DEVICE...]\r\n"
                                           "  STDIN   - redirect only STDIN\r\n"
                                           "  STDOUT  - redirect only STDOUT\r\n"
                                           "  DEVICE  - list of device names:\r\n"
                                           "             COMm CON LPT\r\n";

const unsigned char *usage_message = "Change STDIN/STDOUT to local and/or serial device\r\n\n";

void show_help_message(void) {
  printf(usage_message);
  printf(error_usage_message);
}
void show_usage_message(void) { printf(error_usage_message); }

uint8_t map_com_port(const char *const argv, uint8_t port_count, const uint8_t port_number) {

  char device_name[5];
  sprintf(device_name, "COM%d", port_number);

  if (strcmp(argv, device_name) == 0) {
    if (activate_com_port) {
      show_usage_message();
      exit(255);
      return 255;
    }

    if (port_count < port_number) {
      printf("\r\nInvalid COM port number %d\r\n", port_number);
      if (port_count > 0)
        printf("Installed COM port numbers: %d\r\n\n", port_count);
      else
        printf("No COM ports currently installed\r\n\n");

      show_usage_message();
      exit(255);
      return 255;
    }

    activate_com_port = port_number;
    return 255;
  }

  return 0;
}

uint8_t process_cli_arguments(const int argc, char **argv) {
  if (argc <= 1) {
    show_usage_message();
    return 255;
  }

  uint8_t port_count = 0;
  serial_get_available_ports(&port_count);

  for (uint8_t i = 1; i < argc; i++) {
    upcase_string(argv[i]);

    if (strcmp(argv[i], "HELP") == 0 || strcmp(argv[i], "/?") == 0) {
      show_help_message();
      return 255;
    }

    if (strcmp(argv[i], "STDIN") == 0) {
      if (apply_to_stdin) {
        show_usage_message();
        return 255;
      }

      apply_to_stdin = 1;
      continue;
    }

    if (strcmp(argv[i], "STDOUT") == 0) {
      if (apply_to_stdout) {
        show_usage_message();
        return 255;
      }
      apply_to_stdout = 1;
      continue;
    }

    if (strcmp(argv[i], "CON") == 0) {
      if (activate_con) {
        show_usage_message();
        return 255;
      }
      activate_con = 1;
      continue;
    }

    if (map_com_port(argv[i], port_count, 1)) {
      continue;
    }

    if (map_com_port(argv[i], port_count, 2)) {
      continue;
    }

    if (map_com_port(argv[i], port_count, 3)) {
      continue;
    }

    if (map_com_port(argv[i], port_count, 4)) {
      continue;
    }

    show_usage_message();
    return 255;

    // if (strcmp(argv[i], "LPT") == 0) {
    //   if (activate_com_port) {
    //     show_usage_message();
    //     return 255;
    //   }
    //   activate_com_port = 5;
    // }
  }

  if (!apply_to_stdin && !apply_to_stdout) {
    apply_to_stdin  = 1;
    apply_to_stdout = 1;
  }

  return 0;
}
