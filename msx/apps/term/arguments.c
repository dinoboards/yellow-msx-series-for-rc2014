#include "arguments.h"
#include "system-state.h"

const char *pFossilBaudRates[12] = {"75", "300", "600", "1200", "2400", "4800", "9600", "19200", "38400", "57600", "unknown", "115200"};

const unsigned char *usage = "Usage:  term [options]\r\n\n"
                             "Connect to a remote BBS system using\r\n"
                             "fossil serial driver\r\n\n"
                             "/a       turn off automatic download\r\n"
                             "         detection\r\n"
                             "/o       turn off ANSI rendering and\r\n"
                             "         use MSX-DOS text rendering\r\n"
                             "/r       Use alternative download\r\n"
                             "         method\r\n"
                             "/i       Display information and\r\n"
                             "         instructions\r\n"
                             "/b<rate> select baud rate\r\n";

uint8_t abort_with_help() {
  printf(usage);
  exit(1);

  return 255;
}

uint8_t arg_help_msg(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  if (strcmp(arg_switch, "/h") != 0 && strcasecmp(arg_switch, "/help") != 0)
    return i;

  return abort_with_help();
}

uint8_t arg_info_msg(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  if (strcmp(arg_switch, "/i") != 0)
    return i;

  printf("TERM.COM written for Yellow MSX Series\r\n"
         "designed for RC2014\r\n"
         "Version 0.0.1\r\n"
         "By Dean Netherton\r\n\n"
         "Converted from MSX BaDCaT ANSI Terminal v1.34\r\n"
         "(c) 2020 Oduvaldo Pavan Junior - ducasp@gmail.com\r\n"
         "Contributor: Andres Ortiz\r\n\n"
         "F1 ---------> Download File\r\n"
         "F2 ---------> Toogle Local echo\r\n"
         "F3 ---------> Toggle CR/CRLF\r\n"
         "CTRL-BREAK -> Exit\r\n");

  exit(1);
  return 255;
}

uint8_t abort_with_invalid_arg_msg(const uint8_t i, const char **argv) {
  printf("Invalid command line option '");
  printf((char *)argv[i]);
  printf("'\r\n\r\n");
  return abort_with_help();
}

uint8_t arg_automatic_download(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  if (strcmp(arg_switch, "/a") != 0)
    return i;

  ucAutoDownload = 0;
  return i + 1;
}

uint8_t arg_disable_ansi(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  if (strcmp(arg_switch, "/o") != 0)
    return i;

  ucAnsi = 0;
  return i + 1;
}

uint8_t arg_alternative_download_method(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  if (strcmp(arg_switch, "/r") != 0)
    return i;

  ucStandardDataTransfer = 0;
  return i + 1;
}

uint8_t arg_baud_rate(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  const char *arg_value;
  if (strncasecmp(arg_switch, "/baud", 5) == 0) {
    if (arg_switch[5] != '=') {
      printf("Invalid baud switch - use /baud=<rate>\r\n");
      return abort_with_help();
    }

    arg_value = arg_switch + 6;
    goto process_baud_rate;
  } else if (strncmp(arg_switch, "/b", 2) == 0) {
    arg_value = arg_switch + 2;
    goto process_baud_rate;
  } else
    return i;

process_baud_rate:
  if (strcmp(arg_value, "75") == 0) {
    baud_rate = 0;
    return i + 1;
  }

  if (strcmp(arg_value, "300") == 0) {
    baud_rate = 1 * 256 + 1;
    return i + 1;
  }

  if (strcmp(arg_value, "600") == 0) {
    baud_rate = 2 * 256 + 2;
    return i + 1;
  }

  if (strcmp(arg_value, "1200") == 0) {
    baud_rate = 3 * 256 + 3;
    return i + 1;
  }

  if (strcmp(arg_value, "2400") == 0) {
    baud_rate = 4 * 256 + 4;
    return i + 1;
  }

  if (strcmp(arg_value, "4800") == 0) {
    baud_rate = 5 * 256 + 5;
    return i + 1;
  }

  if (strcmp(arg_value, "9600") == 0) {
    baud_rate = 6 * 256 + 6;
    return i + 1;
  }

  if (strcmp(arg_value, "19200") == 0) {
    baud_rate = 7 * 256 + 7;
    return i + 1;
  }

  if (strcmp(arg_value, "38400") == 0) {
    baud_rate = 8 * 256 + 8;
    return i + 1;
  }

  if (strcmp(arg_value, "57600") == 0) {
    baud_rate = 9 * 256 + 9;
    return i + 1;
  }

  if (strcmp(arg_value, "115200") == 0) {
    baud_rate = 11 * 256 + 11;
    return i + 1;
  }

  printf("Invalid baud rate setting '");
  printf(arg_value);
  printf("'\r\n");
  exit(1);

  return 255;
}

void process_cli_arguments(const int argc, const char **argv) {
  ucAutoDownload = 1;
  ucStandardDataTransfer = 1;
  ucAnsi = 1;

  for (uint8_t i = 1; i < argc;) {
    const uint8_t current_i = i;

    i = arg_help_msg(i, argv);
    if (current_i != i)
      continue;

    i = arg_info_msg(i, argv);
    if (current_i != i)
      continue;

    i = arg_automatic_download(i, argv);
    if (current_i != i)
      continue;

    i = arg_disable_ansi(i, argv);
    if (current_i != i)
      continue;

    i = arg_alternative_download_method(i, argv);
    if (current_i != i)
      continue;

    i = arg_baud_rate(i, argv);
    if (current_i != i)
      continue;

    abort_with_invalid_arg_msg(i, argv);
  }
}
