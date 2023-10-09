#include "ansiprint.h"
#include "msx2ansi.h"

unsigned char usingAnsiDrv;

void initPrint(void) { usingAnsiDrv = 0; }

void StartPrintBuffer(void) {
  if (usingAnsiDrv)
    AnsiStartBuffer();
}

void EndPrintBuffer(void) {
  if (usingAnsiDrv)
    AnsiEndBuffer();
}

void initAnsi(unsigned int uiCallBackFunction) {
  usingAnsiDrv = 1;
  AnsiInit();
  AnsiCallBack(uiCallBackFunction);
}

void endAnsi(void) {
  usingAnsiDrv = 0;
  AnsiFinish();
}
