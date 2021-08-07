#ifndef __SYSTEM_STATE
#define __SYSTEM_STATE

// Our Flags
extern unsigned char ucEcho;         // Echo On?
extern unsigned char ucAutoDownload; // Auto download on binary transfers?
extern unsigned char ucAnsi;         // Using ANSI rendering?
// extern unsigned char ucCP437;                // Using custom CP437 if MSX1?
extern unsigned char ucEnterHit;             // user has input enter?
extern unsigned char ucWidth40;              // Detected 40 Columns or less?
extern unsigned char ucState;                // State of Telnet Data Parser
extern unsigned char ucCmdCounter;           // If there is a TELNET command in progress, its size
extern unsigned char ucStandardDataTransfer; // Is this telnet server proper and transmitting files using telnet double FF?
extern unsigned char ucConnNumber;           // hold the connection number received by UnapiHelper

// For data receive parsing
extern unsigned char ucRcvData[128];

#define RcvMemorySize 1024
extern unsigned char ucRcvDataMemory[RcvMemorySize];
extern unsigned int  uiGetSize;

extern const unsigned char *ucServer;  // will hold the name of the server we will connect
extern unsigned char        ucPort[6]; // will hold the port that the server accepts connections

#endif
