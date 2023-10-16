#ifndef SCREEN_CONTROL_H
#define SCREEN_CONTROL_H

#include "msxdos.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint8_t screenMode;
  uint8_t screenWidth;
  bool    functionKeysVisible;
} screenConfiguration;

extern screenConfiguration currentScreenConfig;
extern uint8_t             screenLinesCount;

#define clearScreen()                    printf("\x0C")
#define homeCursor()                     printf("\x0D\x1BK")
#define cursorDown()                     printf("\x1F")
#define deleteToEndOfLine()              printf("\x1BK")
#define deleteToEndOfLineAndCursorDown() printf("\x1BK\x1F");
#define newLine()                        printf("\x0A\x0D");
#define hideCursor()                     printf("\x1Bx5")
#define displayCursor()                  printf("\x1By5")

extern void saveOriginalScreenConfiguration(void);
extern void composeWorkScreenConfiguration(void);
extern void setScreenConfiguration(void);
extern void locateX(uint8_t x);
extern void locate(uint8_t x, uint8_t y);
extern void printCentered(char *string);

#endif
