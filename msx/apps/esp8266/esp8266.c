#include "extbio.h"
#include "fossil.h"
#include "print.h"
#include "system_vars.h"
#include <extbio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ONE_AND_HALF_SECONDS (VDP_FREQUENCY * 1.5)

#define delay_reached (delay_point - ((int16_t)JIFFY) >= 0)

void delay(uint8_t period) {
  const int16_t delay_point = ((int16_t)JIFFY) + (period);
  while (!delay_reached)
    ;
  ;
}

/*
1. Establish fossil serial connection
2. Issue +++ to enter command mode
3. Issue command
4. Write out response

*/
void main(const int argc, const unsigned char **argv) {
  (void)argc;
  (void)argv;
  if (!fossil_link()) {
    extbio_fossil_install();

    if (!fossil_link()) {
      print_str("Fossil driver not found\r\n");
      exit(1);
    }
  }

  print_str("MSX/RC2014 Serial to ESP8266 Control\r\n");

  uint16_t actual_baud_rate = fossil_set_baud(7); // 19200
  fossil_set_protocol(7);                         // 8N1
  fossil_init();

  delay(ONE_AND_HALF_SECONDS);
  fossil_rs_string("+++");
  delay(ONE_AND_HALF_SECONDS);

  fossil_rs_string("ATD192.168.86.46:3000\r\n");

  unsigned int i = 65000;

  while (i != 0) {
    i--;

    if (fossil_rs_in_stat() != 0) {
      const unsigned char t = fossil_rs_in();
      printf("%c", t);
      i = 65000;
    }
  }

  fossil_deinit();
}
