#include "apply_com.h"
#include "apply_width.h"
#include "argument.h"

uint8_t main(const int argc, char *argv[]) {
  uint8_t result = process_cli_arguments(argc, argv);

  if (result)
    return result;

  switch (command_mode) {
  case SET_COM:
    return apply_com_settings();

  case SET_WIDTH:
    return apply_width_settings();
  }

  return 0;
}
