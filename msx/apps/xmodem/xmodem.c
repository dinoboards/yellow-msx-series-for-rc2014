#include "arguments.h"

void main(const uint16_t argc, const char *argv[]) {
  process_cli_arguments(argc, argv);

  if (receive_file) {
    printf("Receiving file '");
    printf(pFileName);
    printf("' on COM1\r\n");
    // receive_xmodem_file(pFileName);
    return;
  }

  if (send_file) {
    printf("Sending file '");
    printf(pFileName);
    printf("' to COM1\r\n");
    // send_xmodem_file(pFileName);
    return;
  }
}
