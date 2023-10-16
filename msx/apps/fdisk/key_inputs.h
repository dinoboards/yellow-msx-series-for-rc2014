#ifndef __KEY_INPUTS_H__
#define __KEY_INPUTS_H__

#include <stdbool.h>
#include <stdint.h>

extern char    getKey(void);
extern uint8_t waitKey(void);
extern bool    getYesOrNo(void);

#define ESC          27
#define CURSOR_RIGHT 28
#define CURSOR_LEFT  29
#define KEY_1        49

#endif
