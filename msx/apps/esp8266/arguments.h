#ifndef __ARGUMENTS
#define __ARGUMENTS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUB_COMMAND_TIME_SYNC    1
#define SUB_COMMAND_HANGUP       2
#define SUB_COMMAND_SET_TIMEZONE 3
#define SUB_COMMAND_SET_WIFI     4
#define SUB_COMMAND_WGET         5
#define SUB_COMMAND_MSXHUB       6

extern uint8_t     subCommand;
extern const char *pNewTimeZone;
extern const char *pNewSSID;
extern const char *pNewPassword;
extern const char *pWgetUrl;
extern const char *pFileName;
extern uint16_t    baud_rate;
extern const char *pMsxHubPackageName;
extern uint8_t     requestLargePacket;

void process_cli_arguments(const char **argv, const int argc);

extern const char *pFossilBaudRates[12];
#define fossil_rate_to_string(f) pFossilBaudRates[f & 0xF]

#endif
