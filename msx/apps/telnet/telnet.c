
#include "ansi_codes.h"
#include "msx2ansi.h"
#include "msx_fusion.h"
#include "print.h"

void SendCursorPosition(unsigned int uiCursorPosition) __z88dk_fastcall { (void)uiCursorPosition; }

void main() {
  AnsiInit();
  AnsiCallBack((unsigned int)SendCursorPosition);

  AnsiPrint(GRN "This is in green\r\n" RED "This is in red\r\n");

  while (Inkey() == 0)
    ;

  AnsiFinish();
}
