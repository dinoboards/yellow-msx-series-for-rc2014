#include "instructions.h"

// Instructions
const char ucUsage[] = "Usage: telnet server[:port] [a] [r] [o] [c]\r\n\r\n"
                       "server[:port]: 192.168.0.1:34 or bbs.hispamsx.org\r\n\r\n"
                       "a: turns off automatic download detection (some BBSs can't be used with it)\r\n"
                       "o: turns off ANSI rendering and use MSX-DOS text rendering\r\n"
                       "c: turns off custom CP437 font on MSX1\r\n"
                       "r: if file transfer fails try using this, some BBSs misbehave on file transfers\r\n\r\n";

const char ucSWInfo[] = "> MSX BaDCaT ANSI Terminal v1.34 <\r\n(c) 2020 Oduvaldo Pavan Junior - ducasp@gmail.com\r\nContributor: Andres Ortiz\r\n\r\n";
const char ucSWInfoANSI[] = "\x1b[37m> MSX BaDCaT ANSI Terminal v1.34 <\r\n\x1b[31m](c) 2020 Oduvaldo Pavan Junior - ducasp@gmail.com\r\nContributor: Andres Ortiz\r\n\r\n";
