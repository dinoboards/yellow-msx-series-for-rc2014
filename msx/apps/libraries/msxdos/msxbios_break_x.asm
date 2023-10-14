	include "msxdos.inc"

	SECTION	CODE

; extern bool msxbiosBreakX();
	PUBLIC	_msxbiosBreakX
_msxbiosBreakX:
	PUSH	IX
	LD	IY, (EXPTBL-1)
	LD	IX, BREAKX
	CALL	CALSLT
	EI
	LD	L, 0
	POP	IX
	RET	NC
	LD	L, 1
	RET
