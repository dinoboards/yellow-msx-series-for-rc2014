#ifndef __WGET
#define __WGET

extern void  wget();
extern void  subCommandWGet();
extern char *waitForMessage;

extern char backupPacket[1025];

#define lastCapturedPacket backupPacket

#endif
