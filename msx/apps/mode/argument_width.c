#include "argument_width.h"
#include "argument.h"
#include <stdint.h>
#include <stdio.h>

uint16_t col_width;

uint8_t process_con_width(const int argc, char **argv) {
  command_mode = SET_WIDTH;

  if (argc == 2) {
    if (sscanf(argv[1], "%d", &col_width) == 1) { // If conversion was successful
      if (col_width >= 1 && col_width <= 80)
        return 0;

      printf("Width must be between 1 and 80\r\n");
      return 255;
    }
  }

  return 0;
}
