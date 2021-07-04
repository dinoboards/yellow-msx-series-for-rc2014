#ifndef __ANSI_PRINT
#define __ANSI_PRINT

#include "print.h"

void StartPrintBuffer();
void EndPrintBuffer();
void initPrint();
void initAnsi(unsigned int uiCallBackFunction);
void endAnsi();

#endif
