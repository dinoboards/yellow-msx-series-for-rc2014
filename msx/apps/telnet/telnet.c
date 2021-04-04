
#include "msx2ansi.h"
#include "print.h"
#include "msx_fusion.h"
#include "ansi_codes.h"

void SendCursorPosition(unsigned int uiCursorPosition) __z88dk_fastcall {
  (void)uiCursorPosition;
}

void main() {
  AnsiInit();
  AnsiCallBack((unsigned int)SendCursorPosition);

  AnsiPrint(GRN "This is in green\r\n" RED "This is in red\r\n");

  while(Inkey() == 0);

  AnsiFinish();
}
