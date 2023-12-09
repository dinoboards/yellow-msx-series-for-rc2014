#include "arguments.h"
#include <ctype.h>
#include <extbio/serial.h>
#include <stdio.h>
#include <string.h>

const char *pFileName    = NULL;
uint8_t     receive_file = false;
uint8_t     send_file    = false;
uint8_t     port_number  = 0;

/*
  TO SAVE A FILE RECEIVED VIA XMODEM ON COM1
    XMODEM COM1 FILENAME

  TO SEND A FILE VIA XMODEM TO COM1
    XMODEM FILENAME COM1

  ASSUME COM1 HAS BEEN CONFIGURED WITH MODE.COM ALREADY (BAUD RATE/BIT PROTOCOL ETC)
*/

void upcase_string(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = toupper((unsigned char)str[i]);
  }
}

uint8_t abort_with_help(void) {
  printf("Usage: \r\n\r\n"
         " XMODEM COMn <filename>\r\n"
         "   receive file on COMm and save\n\r\n"
         " XMODEM <filename> COMn\r\n"
         "   send file to COMm port\n\r\n");

  exit(1);

  return 255;
}

uint8_t abort_with_invalid_options(void) {
  printf("Invalid usage\r\n");
  return abort_with_help();
}

uint8_t is_a_com_port_number(const char *const argv) {
  if (strncmp(argv, "COM", 3) != 0)
    return false;

  if (argv[3] < '1' || argv[3] > '4')
    return false;

  if (argv[4] != 0)
    return false;

  port_number = argv[3] - '0';

  return true;
}

void process_cli_arguments(const uint16_t argc, char **const argv) {
  argv;
  if (argc != 3)
    abort_with_invalid_options();

  upcase_string(argv[1]);
  upcase_string(argv[2]);

  if (is_a_com_port_number(argv[1])) {
    pFileName    = argv[2];
    receive_file = true;
  }

  if (is_a_com_port_number(argv[2])) {
    pFileName = argv[1];
    send_file = true;
  }

  if (!send_file && !receive_file)
    abort_with_invalid_options();

  if (send_file && receive_file)
    abort_with_invalid_options();

  if (pFileName[0] == 0)
    abort_with_invalid_options();

  uint8_t available_ports = 0;
  serial_get_available_ports(&available_ports);

  if (available_ports == 0) {
    printf("No serial ports available\r\n");
    abort_with_invalid_options();
  }

  if (port_number > available_ports) {
    printf("Invalid port number %d. Only %d port(s) installed\r\n", port_number, available_ports);
    abort_with_invalid_options();
  }
}
