#ifndef __ANSI_PRINT
#define __ANSI_PRINT

#include "print.h"

void StartPrintBuffer(void);
void EndPrintBuffer(void);
void initPrint(void);
void initAnsi(unsigned int uiCallBackFunction);
void endAnsi(void);

#endif
