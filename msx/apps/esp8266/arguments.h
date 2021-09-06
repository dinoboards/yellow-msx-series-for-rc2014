#ifndef __ARGUMENTS
#define __ARGUMENTS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const char *pFileName;
extern uint16_t    baud_rate;
extern const char *pDialAddress;

extern const char *pFossilBaudRates[12];

#define fossil_rate_to_string(f) pFossilBaudRates[f & 0xF]

void process_cli_arguments(const char **argv, const int argc);

#endif
