#include "argument.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <tty/tty.h>

uint8_t main(const int argc, const char *argv[]) {
  uint8_t result = process_cli_arguments(argc, argv);

  if (result)
    return result;

  printf("apply_to_stdin: %02X\r\n", apply_to_stdin);
  printf("apply_to_stdout: %02X\r\n", apply_to_stdout);
  printf("activate_com_port: %02X\r\n", activate_com_port);
  printf("activate_con: %02X\r\n", activate_con);

  tty_settings_t tty_settings;
  memset(&tty_settings, 0, sizeof(tty_settings_t));

  if (apply_to_stdin) {
    tty_settings.inputs.console = activate_con;
    tty_settings.inputs.com     = activate_com_port;
  }

  if (apply_to_stdout) {
    tty_settings.outputs.console = activate_con;
    tty_settings.outputs.com     = activate_com_port;
  }

  const uint8_t r = tty_update(&tty_settings);

  if (r != 0) {
    printf("Error: %02X\r\n", r);
    return 255;
  }

  return 0;
}
