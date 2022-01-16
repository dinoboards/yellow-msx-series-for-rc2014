#include "system-state.h"

// Our Flags
unsigned char ucEcho;                 // Echo On?
unsigned char ucAutoDownload;         // Auto download on binary transfers?
unsigned char ucAnsi;                 // Using ANSI rendering?
unsigned char ucEnterHit;             // user has input enter?
unsigned char ucWidth40;              // Detected 40 Columns or less?
unsigned char ucState;                // State of Telnet Data Parser
unsigned char ucCmdCounter;           // If there is a TELNET command in progress, its size
unsigned char ucStandardDataTransfer; // Is this telnet server proper and transmitting files using telnet double FF?

// For data receive parsing
unsigned char ucRcvData[128];

unsigned char ucRcvDataMemory[RcvMemorySize];
unsigned int  uiGetSize;

uint16_t baud_rate = 7 * 256 + 7; // 19200
