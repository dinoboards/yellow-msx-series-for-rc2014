#include "argument.h"
#include <extbio/serial.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// MODE COMm[:] [BAUD=b] [PARITY=p] [DATA=d] [STOP=s]

/*
PARITY=p
    Specifies how the system uses the parity bit to check for transmission
    errors. The p value can be one of the following: N (none), E (even), O
    (odd), M (mark), or S (space). The default value is E. Not all computers
    support the values M and S.

DATA=d
    Specifies the number of data bits in a character. Valid values for d are
    in the range 5 through 8. The default value is 8. Not all computers
    support the values 5 and 6.

STOP=s
    Specifies the number of stop bits that define the end of a character: 1,
    1.5, or 2. If the baud rate is 110, the default value is 2; otherwise,
    the default value is 1. Not all computers support the value 1.5.

*/

uint32_t baud_rate;
uint16_t parity; // one of SERIAL_PARITY_NONE, SERIAL_PARITY_ODD, SERIAL_PARITY_EVEN, SERIAL_PARITY_MARK, SERIAL_PARITY_SPACE
uint16_t stop_bit_count; // one of SERIAL_STOPBITS_1, SERIAL_STOPBITS_15, SERIAL_STOPBITS_2
uint8_t  port_number;
uint8_t  data_bits;

#define process_port_optional_args process_port_baud

uint8_t process_unknown_arg(const char *argv) {
  printf("Unknown argument: %s\r\n", argv);
  return 255;
}

uint8_t process_port_stop(const char *argv) {
  if (strncmp(argv, "STOP=", 5) != 0)
    return process_unknown_arg(argv);

  if (strcmp(argv + 5, "1") == 0) {
    stop_bit_count = SERIAL_STOPBITS_1;
    return 0;
  }

  if (strcmp(argv + 5, "1.5") == 0) {
    stop_bit_count = SERIAL_STOPBITS_15;
    return 0;
  }

  if (strcmp(argv + 5, "2") == 0) {
    stop_bit_count = SERIAL_STOPBITS_2;
    return 0;
  }

  printf("Invalid stop bit count. It must be 1, 1.5, or 2.\r\n");
  return 255;
}

uint8_t process_port_data(const char *argv) {
  if (strncmp(argv, "DATA=", 5) == 0) {
    data_bits = atoi(argv + 5);
    if (data_bits < 5 || data_bits > 8) {
      printf("Invalid data bits value. It must be between 5 and 8.\r\n");
      return 255;
    }
    return 0;
  }

  return process_port_stop(argv);
}

uint8_t process_port_parity(const char *argv) {
  if (strncmp(argv, "PARITY=", 7) == 0) {
    const char *parity_value = argv + 7;
    if (strcmp(parity_value, "N") == 0 || strcmp(parity_value, "NONE") == 0) {
      parity = SERIAL_PARITY_NONE;
    } else if (strcmp(parity_value, "E") == 0 || strcmp(parity_value, "EVEN") == 0) {
      parity = SERIAL_PARITY_EVEN;
    } else if (strcmp(parity_value, "O") == 0 || strcmp(parity_value, "ODD") == 0) {
      parity = SERIAL_PARITY_ODD;
    } else if (strcmp(parity_value, "M") == 0 || strcmp(parity_value, "MARK") == 0) {
      parity = SERIAL_PARITY_MARK;
    } else if (strcmp(parity_value, "S") == 0 || strcmp(parity_value, "SPACE") == 0) {
      parity = SERIAL_PARITY_SPACE;
    } else {
      printf("Invalid parity value. It must be NONE, EVEN, ODD, MARK, or SPACE.\r\n");
      return 255;
    }

    return 0;
  }

  return process_port_data(argv);
}

uint8_t process_port_baud(const char *argv) {
  if (strncmp(argv, "BAUD=", 5) == 0) {
    baud_rate = atol(argv + 5);
    return 0;
  }

  return process_port_parity(argv);
}

uint8_t process_port(const int argc, char **argv) {
  command_mode = SET_COM;

  if (strncmp(argv[1], "COM", 3) == 0) {
    port_number = argv[1][3] - '0';
    if (port_number < 1 || port_number > 4) {
      printf("Invalid COM port_number number. It must be between 1 and 4.\r\n");
      return 255;
    }
  }

  for (int i = 2; i < argc; i++) {
    if (process_port_optional_args(argv[i])) {
      printf("Usage: mode COMm[:] [BAUD=b] [PARITY=p] [DATA=d] [STOP=s] [RETRY=r]\r\n");

      return 255;
    }
  }

  printf("COM port_number number: %d\r\n", port_number);
  printf("Baud rate: %ld\r\n", baud_rate);
  printf("Parity: %04X\r\n", parity);
  printf("Stop bit count: %04X\r\n", stop_bit_count);

  return 0;
}
