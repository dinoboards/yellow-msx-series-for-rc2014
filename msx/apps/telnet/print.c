#include "print.h"
#include "asm.h"
#include "msx2ansi.h"

unsigned char usingAnsiDrv;

void initPrint() { usingAnsiDrv = 0; }

void StartPrintBuffer() {
  if (usingAnsiDrv)
    AnsiStartBuffer();
}

void EndPrintBuffer() {
  if (usingAnsiDrv)
    AnsiEndBuffer();
}

void initAnsi(unsigned int uiCallBackFunction) {
  usingAnsiDrv = 1;
  AnsiInit();
  AnsiCallBack(uiCallBackFunction);
}

void endAnsi() {
  usingAnsiDrv = 0;
  AnsiFinish();
}
