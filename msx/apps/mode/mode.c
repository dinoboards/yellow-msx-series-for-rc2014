#include <ctype.h>
#include <extbio/serial.h>
#include <msxbios/msxbios.h>
#include <msxbios/system_vars.h>
#include <stdio.h>
#include <string.h>

// MODE COMm[:] [BAUD=b] [PARITY=p] [DATA=d] [STOP=s]

/*
PARITY=p
    Specifies how the system uses the parity bit to check for transmission
    errors. The p value can be one of the following: N (none), E (even), O
    (odd), M (mark), or S (space). The default value is E. Not all computers
    support the values M and S. You can abbreviate this parameter by
    omitting PARITY= and specifying a value for p.

DATA=d
    Specifies the number of data bits in a character. Valid values for d are
    in the range 5 through 8. The default value is 8. Not all computers
    support the values 5 and 6. You can abbreviate this parameter by
    omitting DATA= and specifying a value for d.

STOP=s
    Specifies the number of stop bits that define the end of a character: 1,
    1.5, or 2. If the baud rate is 110, the default value is 2; otherwise,
    the default value is 1. Not all computers support the value 1.5. You can
    abbreviate this parameter by omitting STOP= and specifying a value for
    s.

*/

typedef enum { SET_COM = 1, SET_WIDTH = 2 } command_mode_t;

uint32_t baud_rate;
uint16_t parity; // one of SERIAL_PARITY_NONE, SERIAL_PARITY_ODD, SERIAL_PARITY_EVEN, SERIAL_PARITY_MARK, SERIAL_PARITY_SPACE
uint16_t stop_bit_count; // one of SERIAL_STOPBITS_1, SERIAL_STOPBITS_15, SERIAL_STOPBITS_2
uint8_t  com_port;
uint8_t  data_bits;
uint16_t col_width;

command_mode_t command_mode;

void upcase_string(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = toupper((unsigned char)str[i]);
  }
}

#define process_com_port_optional_args process_com_port_baud

uint8_t process_unknown_arg(const char *argv) {
  printf("Unknown argument: %s\r\n", argv);
  return 255;
}

uint8_t process_com_port_stop(const char *argv) {
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

uint8_t process_com_port_data(const char *argv) {
  if (strncmp(argv, "DATA=", 5) == 0) {
    data_bits = atoi(argv + 5);
    if (data_bits < 5 || data_bits > 8) {
      printf("Invalid data bits value. It must be between 5 and 8.\r\n");
      return 255;
    }
    return 0;
  }

  return process_com_port_stop(argv);
}

uint8_t process_com_port_parity(const char *argv) {
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

  return process_com_port_data(argv);
}

uint8_t process_com_port_baud(const char *argv) {
  if (strncmp(argv, "BAUD=", 5) == 0) {
    baud_rate = atol(argv + 5);
    return 0;
  }

  return process_com_port_parity(argv);
}

uint8_t process_com_port(const int argc, char **argv) {
  command_mode = SET_COM;

  if (strncmp(argv[1], "COM", 3) == 0) {
    com_port = argv[1][3] - '0';
    if (com_port < 1 || com_port > 4) {
      printf("Invalid COM port number. It must be between 1 and 4.\r\n");
      return 255;
    }
  }

  for (int i = 2; i < argc; i++) {
    if (process_com_port_optional_args(argv[i])) {
      printf("Usage: mode COMm[:] [BAUD=b] [PARITY=p] [DATA=d] [STOP=s] [RETRY=r]\r\n");

      return 255;
    }
  }

  printf("COM port number: %d\r\n", com_port);
  printf("Baud rate: %ld\r\n", baud_rate);
  printf("Parity: %04X\r\n", parity);
  printf("Stop bit count: %04X\r\n", stop_bit_count);

  return 0;
}

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

uint8_t process_cli_arguments(const int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: mode COMm[:] [BAUD=b] [PARITY=p] [DATA=d] [STOP=s] [RETRY=r]\r\n");
    return 255;
  }

  for (int i = 1; i < argc; i++)
    upcase_string(argv[i]);

  if (strncmp(argv[1], "COM", 3) == 0)
    return process_com_port(argc, argv);

  if (process_con_width(argc, argv) == 0)
    return 0;

  printf("Usage: mode COMm[:] [BAUD=b] [PARITY=p] [DATA=d] [STOP=s] [RETRY=r]\r\n");
  return 255;
}

uint8_t main(const int argc, char *argv[]) {
  uint8_t result = process_cli_arguments(argc, argv);

  printf("Result %d\r\n", result);

  if (result)
    return result;

  switch (command_mode) {
  case SET_COM:
    printf("Setting COM port\r\n");
    break;
  case SET_WIDTH: {
    if (col_width >= 33) {
      LINL40 = col_width;
      msxbiosInitxt();
    } else {
      LINL32 = col_width;
      msxbiosInit32();
    }
    break;
  }
  }

  return 0;
}
