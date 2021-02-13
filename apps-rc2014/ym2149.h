#ifndef __YM2149
#define __YM2149

#include <stdbool.h>
#include <stdlib.h>

// #include "audio.h"
// #include "datatypes.h"

typedef struct _audioCommand {
  uint8_t  channel; // 2 bits
  uint8_t  volume;  // 4 bits
  uint16_t period;  // 12 bits
} audioCommand;

extern audioCommand command;

extern bool ym2149Probe();
extern void ym2149Play(uint16_t period) __z88dk_fastcall;
extern void ym2149Stop();

#define ym2149Init() (command.channel = 1)

extern uint8_t getControllerDirection(uint8_t port) __z88dk_fastcall;

// ; FUNCTION : RETURNS CURRENT TRIGGER STATUS
// ;	1 = PORT 1, BUTTON A
// ;	2 = PORT 2, BUTTON A
// ;	3 = PORT 1, BUTTON B
// ;	4 = PORT 2, BUTTON B
// returns true (1) if button pressed.
extern uint8_t getControllerButton(uint8_t b) __z88dk_fastcall;

#endif
