#include <stdio.h>
#include <extbio.h>
#include "arguments.h"

void main(const int argc, const unsigned char **argv) {
  process_cli_arguments(argc, argv);

  const uint8_t is_present = extbio_sio_get_present();

  if (is_present) {
    const uint8_t org_clock_code2 = extbio_sio_set_clock(clock_speed_code);

    printf("SIO/2:           %s\r\n", clock_speed);
  }
}
