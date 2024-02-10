#include "arguments.h"
#include <msxbios.h>
#include <stdint.h>
#include <stdio.h>
#include <xmodem/receive.h>

uint32_t retrieved_bytes;

void main(const uint16_t argc, const char *argv[]) {
  process_cli_arguments(argc, argv);

  if (arg_receive_file) {
    printf("Downloading '%s' ", arg_p_file_name);
    if (xmodem_receive(arg_p_file_name, &retrieved_bytes, false) == 0)
      printf(ERASE_LINE "Downloaded '%s'\r\n", arg_p_file_name);
    else
      printf("\r\n");

    return;
  }

  if (arg_send_file) {
    printf("Sending file '");
    printf(arg_p_file_name);
    printf("' to COM1\r\n");
    printf("Not implemented yet\r\n");
    // send_xmodem_file(p_file_name);
    return;
  }
}
