#include "argument.h"
#include <stdint.h>
#include <stdio.h>

uint8_t main(const int argc, const char *argv[]) {
  uint8_t result = process_cli_arguments(argc, argv);

  if (result)
    return result;

  printf("TODO: implement\n");
  printf("redirect_stream: %02X\n", redirect_stream);
  return 0;
}
