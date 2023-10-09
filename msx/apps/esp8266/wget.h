#ifndef __WGET
#define __WGET

extern void  wget(void);
extern void  subCommandWGet(void);
extern char *waitForMessage;

extern char backupPacket[1025];

#define lastCapturedPacket backupPacket

#endif
