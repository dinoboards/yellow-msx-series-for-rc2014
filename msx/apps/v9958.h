#ifndef __V9958
#define __V9958

#include <stdbool.h>
#include <stdlib.h>

#define PAL  1
#define NTSC 2

typedef struct {
  uint8_t red;
  uint8_t blue;
  uint8_t green;
} RGB;

extern void outCmd(uint8_t b) __z88dk_fastcall;
extern void outDat(uint8_t b) __z88dk_fastcall;

extern void clearAllMemory();
extern void setPalette(RGB *) __z88dk_fastcall;
extern void setTextMode();
extern void setMode6(uint8_t lines, uint8_t mode);
extern void setMode7(uint8_t lines, uint8_t mode);
extern void clearScreenBank0(uint8_t color) __z88dk_fastcall;
extern void clearScreenBank1(uint8_t color) __z88dk_fastcall;
extern void _writeRegister(uint16_t rd) __z88dk_fastcall;
extern void vramReadFast(uint32_t vramAddress, uint8_t *buffer, uint16_t size);
extern void vramWriteFast(uint32_t vramAddress, uint8_t *buffer, uint16_t size);

extern void vramReadSlow(uint32_t vramAddress, uint8_t *buffer, uint16_t size);
extern void vramWriteSlow(uint32_t vramAddress, uint8_t *buffer, uint16_t size);

#define writeRegister(a, b) _writeRegister(a * 256 + b)

#define REGISTER_COUNT 12

#define CMD_VDP_TO_VRAM 0xC0
#define CMD_LINE(op)    (0x70 | op)
#define CMD_PSET(op)    (0x50 | op)
#define CMD_LOGIC_IMP   0x00
#define CMD_LOGIC_AND   0x01

void commandDrawLine();
void waitForCommandCompletion();

extern uint8_t  dir;
extern uint16_t longSide;
extern uint16_t shortSide;
extern uint16_t _fromX;
extern uint16_t _fromY;
extern uint8_t  _color;
extern uint8_t  _operation;
extern uint16_t _toX;
extern uint16_t _toY;

#define drawLine(fromX, fromY, toX, toY, color, operation)                                                                         \
  _fromX     = (fromX);                                                                                                            \
  _fromY     = (fromY);                                                                                                            \
  _color     = (color);                                                                                                            \
  _operation = CMD_LINE((operation));                                                                                              \
  _toX       = (toX);                                                                                                              \
  _toY       = (toY);                                                                                                              \
  _drawLine()

extern void _drawLine();

#define pointSet(x, y, color, operation)                                                                                           \
  _fromX     = (x);                                                                                                                \
  _fromY     = (y);                                                                                                                \
  _color     = (color);                                                                                                            \
  _operation = CMD_PSET((operation));                                                                                              \
  commandDrawLine()

#endif
