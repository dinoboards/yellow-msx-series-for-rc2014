
#include "print.h"
#include <msxdos.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t measurement = 0;
void     measure_timing();

static uint32_t clock = 3686400;
static bool     m1_state = false;

#define T_STATES_WITHOUT_M1 77
#define T_STATES_WITH_M1    68
// approx 4 tstate at 3.6864 or 8 states at 7.372800
#define T_STATES_OVERHEAD 0
// (clock / 921600)
#define WAIT_T_STATES     (m1_state ? T_STATES_WITHOUT_M1 : T_STATES_WITH_M1)

uint8_t abort_with_help() {
  print_str("Usage:  rtccalb [options]\r\n\r\n"
            "RTC Calibration Tool\r\n\r\n"
            "  /c<clock>, /cpu=<clock> (Optional)\r\n"
            "    The cpu <clock> eg 3686400\r\n"
            "\r\n"
            "  /m1\r\n"
            "    Assume MSX M1 wait states are\r\n"
            "    active\r\n"
            ""

  );
  exit(1);

  return 255;
}

uint8_t abort_with_invalid_arg_msg(const uint8_t i, const char **argv) {
  print_str("Invalid command line option '");
  print_str((char *)argv[i]);
  print_str("'\r\n\r\n");
  return abort_with_help();
}

uint8_t arg_cpu_clock(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  const char *arg_value;
  if (strncasecmp(arg_switch, "/cpu", 4) == 0) {
    if (arg_switch[4] != '=') {
      print_str("Invalid cpu switch - use /cpu=<clock>\r\n");
      return abort_with_help();
    }

    arg_value = arg_switch + 5;
    goto process_clock_rate;
  } else if (strncmp(arg_switch, "/c", 2) == 0) {
    arg_value = arg_switch + 2;
    goto process_clock_rate;
  } else
    return i;

process_clock_rate:
  clock = atol(arg_value);

  return i + 1;
}

uint8_t arg_help_msg(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  if (strcmp(arg_switch, "/h") != 0 && strcasecmp(arg_switch, "/help") != 0)
    return i;

  return abort_with_help();
}

uint8_t arg_m1_state(const uint8_t i, const char **argv) {
  const char *arg_switch = argv[i];
  if (strcmp(arg_switch, "/m1") != 0)
    return i;

  m1_state = true;
  return i + 1;
}

void process_cli_arguments(const int argc, const char **argv) {
  (void)argc;
  (void)argv;

  // abort_with_help();
  for (uint8_t i = 1; i < argc;) {
    const uint8_t current_i = i;
    i = arg_cpu_clock(i, argv);
    if (current_i != i)
      continue;

    i = arg_help_msg(i, argv);
    if (current_i != i)
      continue;

    i = arg_m1_state(i, argv);
    if (current_i != i)
      continue;

    abort_with_invalid_arg_msg(i, argv);
  }
}

#define ERASE_LINE "\x1B\x6C\r"

void main(const int argc, const unsigned char **argv) {
  process_cli_arguments(argc, argv);

  if (clock == 0) {
    printf("Missing cpu clock rate argument\r\n\r\n");
    abort_with_help();
  }

  printf("RTC Calibration tool v0.0.4\r\n"
         "Yellow MSX RTC/F4 Module\r\n"
         "Designed for RC2014\r\n"
         "==========================\r\n\r\n");

  printf("Measuring RTC clock against\r\n"
         "CPU Clock Rate of %ld,\r\n"
         "with MSX M1-Wait States %s\r\n\r\n"
         "Hold CTRL-BREAK to abort\r\n\r\n",
         clock, m1_state ? "enabled" : "disabled");

  while (!msxbiosBreakX()) {
    measure_timing();

    const uint32_t expect = (clock / WAIT_T_STATES);
    const int8_t   diff = expect - measurement + T_STATES_OVERHEAD;

    if (diff == 0)
      printf(ERASE_LINE "Approx. in sync with CPU clock");
    else if (diff > 0)
      printf(ERASE_LINE "Approx. Fast By %02d/%lu secs", diff, clock);
    else
      printf(ERASE_LINE "Approx. Slow By %02d/%lu secs", -diff, clock);
  }
}
