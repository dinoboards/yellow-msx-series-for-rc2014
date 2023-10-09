#ifndef __CPMCALLS
#define __CPMCALLS

#include <stdlib.h>

/* Return a character without echoing if one is waiting; zero if none is available. */

extern uint8_t cRawIo(void);
extern char    cRead(void);

#endif
