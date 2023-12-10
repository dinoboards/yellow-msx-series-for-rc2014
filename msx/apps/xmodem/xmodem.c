#include "arguments.h"
#include "xmodem-receive.h"
#include <stdint.h>
#include <stdio.h>

void main(const uint16_t argc, const char *argv[]) {
  process_cli_arguments(argc, argv);

  if (receive_file) {
    printf("Receiving file '");
    printf(p_file_name);
    printf("' on COM1\r\n");
    xmodem_receive();
    return;
  }

  if (send_file) {
    printf("Sending file '");
    printf(p_file_name);
    printf("' to COM1\r\n");
    // send_xmodem_file(p_file_name);
    return;
  }
}
