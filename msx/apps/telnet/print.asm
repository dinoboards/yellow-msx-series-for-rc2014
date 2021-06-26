
	SECTION CODE

	EXTERN _usingAnsiDrv, _AnsiPrint, _usingAnsiDrv, _AnsiPutChar
; // This print function has been copied from HGET / Konamiman
; // Using it as fusion-c Print uses bios calls, and do not work with PUT9000
; // That hooks the dos call.
; void print(char* s) __z88dk_fastcall
	PUBLIC	_print
_print:
    ld  a,(_usingAnsiDrv)
    or  a
    jp  nz,_AnsiPrint
    push    ix
loop:
    ld  a,(hl)
    or  a
    jr  z,end
    ld  e,a
    ld  c,2
    push    hl
    call    5
    pop hl
    inc hl
    jr  loop
end:
    pop ix
    ret


; void printChar(char c) __z88dk_fastcall
	PUBLIC	_printChar
_printChar:
    ld  a,(_usingAnsiDrv)
    or  a
    jp  nz,_AnsiPutChar
    push    ix
    ld  e,l
    ld  c,2
    call    5
    pop ix
    ret
