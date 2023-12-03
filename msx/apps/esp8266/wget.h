#ifndef __WGET
#define __WGET

#include <stdint.h>

extern void  wget(void);
extern void  subCommandWGet(void);
extern char *waitForMessage;

#define packetBody     (xmodemState.packetBuffer + 3)
#define packetBodySize (xmodemState.currentPacketSize)

#endif
