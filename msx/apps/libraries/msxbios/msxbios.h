#ifndef _MSXBIOS_H_
#define _MSXBIOS_H_

#include <stdint.h>

extern void msxbiosInit32(void);
extern void msxbiosInitxt(void);
extern void softReset(void);

#define CURSOR_MOVE_LEFT "\x1D"
#define CURSOR_OFF       "\x1B\x78\x35"
#define CURSOR_ON        "\x1B\x79\x35"

#define CHAR_VERT_BAR     "\x01\x56"
#define CHAR_FOWARD_SLASH "/"  /*"\x01\x5D"*/
#define CHAR_DASH         "-"  /*"\x01\x57"*/
#define CHAR_BACK_SLASH   "\\" /*"\x01\x5E"*/

#endif
