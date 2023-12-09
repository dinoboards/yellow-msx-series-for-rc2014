#ifndef _MSXBIOS_H_
#define _MSXBIOS_H_

#include <stdint.h>

extern void msxbiosInit32(void);
extern void msxbiosInitxt(void);
extern void softReset(void);

#define CHAR_ESCAPE     '\x1B'
#define CHAR_MOVE_LEFT  '\x1D'
#define CHAR_MOVE_RIGHT '\x1C'

#define CURSOR_MOVE_LEFT "\x1D"
#define CURSOR_OFF       "\x1B\x78\x35"
#define CURSOR_ON        "\x1B\x79\x35"

#define STR_VERT_BAR     "\x01\x56"
#define STR_FOWARD_SLASH "/"  /*"\x01\x5D"*/
#define STR_DASH         "-"  /*"\x01\x57"*/
#define STR_BACK_SLASH   "\\" /*"\x01\x5E"*/

#endif
