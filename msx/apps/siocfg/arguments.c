#include "arguments.h"

uint8_t clock_speed_code;
const char* clock_speed = "";

const unsigned char *usage = "Usage:  siocfg [options]\r\n\n"
                             "Configure the SIO/2 Clock speed\r\n\n"
                             "/i       Display information and\r\n"
                             "         instructions\r\n"
                             "/h       Display help message\r\n\n"
                             "CLK=<clock-speed>\r\n"
                             "         Set the hardwired clock\r\n"
                             "         speed\r\n";


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

  printf("SIOCFG.COM written for Yellow MSX Series\r\n"
         "designed for RC2014\r\n"
         "Version 0.0.1\r\n"
         "Valid CLK settings:\r\n"
          "  307200  for baud: 4800,9600,19200\r\n"
          "  614400  for baud: 9600,19200,38400\r\n"
          "  921600  for baud: 57600\r\n"
          "  1228800 for baud: 19200,38400\r\n"
          "  2457600 for baud: 38400\r\n"
          "  3686400 for baud: 57600,115200\r\n"
         "\r\n");

  exit(1);
  return 255;
}

uint8_t abort_with_invalid_arg_msg(const uint8_t i, const char **argv) {
  printf("Invalid command line option '");
  printf((char *)argv[i]);
  printf("'\r\n\r\n");
  return abort_with_help();
}

uint8_t arg_clock_rate(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  const char *arg_value;
  if (strncasecmp(arg_switch, "clk", 3) == 0) {
    if (arg_switch[3] != '=') {
      printf("Invalid baud switch - use CLK=<clock-speed>\r\n");
      return abort_with_help();
    }

    arg_value = arg_switch + 4;
    goto process_clock_speed;
  } else
    return i;

process_clock_speed:
  if (strcmp(arg_value, "307200") == 0) {
    clock_speed_code = 1;
    clock_speed = "307.2Khz\r\nBAUD:            4800,9600,19200";
    return i + 1;
  }

  if (strcmp(arg_value, "614400") == 0) {
    clock_speed_code = 2; 
    clock_speed = "614.4Khz\r\nBAUD:            9600,19200,38400";
    return i + 1;
  }

  // if (strcmp(arg_value, "921600") == 0) {
  //   clock_speed_code = 3;
  //   return i + 1;
  // }

  // if (strcmp(arg_value, "1228800") == 0) {
  //   clock_speed_code = 4;
  //   return i + 1;
  // }

  // if (strcmp(arg_value, "2457600") == 0) {
  //   clock_speed_code = 5;
  //   return i + 1;
  // }

  // if (strcmp(arg_value, "3686400") == 0) {
  //   clock_speed_code = 6;
  //   return i + 1;
  // }

  printf("Invalid clock rate setting '");
  printf(arg_value);
  printf("'\r\n");
  exit(1);

  return 255;
}

void process_cli_arguments(const int argc, const char **argv) {
  for (uint8_t i = 1; i < argc;) {
    const uint8_t current_i = i;

    i = arg_help_msg(i, argv);
    if (current_i != i)
      continue;

    i = arg_info_msg(i, argv);
    if (current_i != i)
      continue;

    i = arg_clock_rate(i, argv);
    if (current_i != i)
      continue;

    abort_with_invalid_arg_msg(i, argv);
  }

  if (clock_speed_code == 0) {
    printf("Missing argument CLK=<clock-speed>\r\n");
    exit(1);
  }
}
