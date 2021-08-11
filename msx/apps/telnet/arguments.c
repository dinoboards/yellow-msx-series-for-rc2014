#include "arguments.h"
#include "system-state.h"
const unsigned char *usage = "Usage:  telnet server[:port] [option]\r\n\n"
                             "Connect to a remote BBS system using\r\n"
                             "fossil serial driver\r\n\n"
                             "server  The target domain name or ip\r\n"
                             "port    The optional port number\r\n\n"
                             "/a      turn off automatic download\r\n"
                             "        detection\r\n"
                             "/o      turn off ANSI rendering and\r\n"
                             "        use MSX-DOS text rendering\r\n"
                             "/r      Use alternative download\r\n"
                             "        method\r\n"
                             "/i      Display information and\r\n"
                             "        instructions\r\n";

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

  printf("TELNET.COM written for Yellow MSX Series\r\n"
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

void abort_with_missing_remote_caller() {
  printf("Missing remote server domain name\r\n\r\n");
  abort_with_help();
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

uint8_t arg_remote_caller(const uint8_t i, const char **argv) {
  if (argv[i][0] != '/') {
    if (ucServer)
      return abort_with_invalid_arg_msg(i, argv);

    ucServer = argv[i];
    return i + 1;
  }

  return i;
}

void process_cli_arguments(const int argc, const char **argv) {
  ucServer = NULL;
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

    i = arg_remote_caller(i, argv);
    if (current_i != i)
      continue;

    abort_with_invalid_arg_msg(i, argv);
  }

  if (ucServer == NULL)
    abort_with_missing_remote_caller();

  printf("cli parsed: %s\r\n", ucServer);
}
