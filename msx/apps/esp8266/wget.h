#ifndef __WGET
#define __WGET

#include <stdint.h>

// typedef struct {
//   char backupPacket[1025];
//   int16_t backupPacketSize;
// } wget_state_t;

extern void  wget(void);
extern void  subCommandWGet(void);
extern char *waitForMessage;

extern char    backupPacket[1025];
extern int16_t backupPacketSize;

// #define lastCapturedPacket backupPacket

#endif
