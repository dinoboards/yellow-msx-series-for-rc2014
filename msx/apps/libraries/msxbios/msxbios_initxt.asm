

	include "msx.inc"

	SECTION	CODE

; void msxbiosInitxt();
	PUBLIC	_msxbiosInitxt
_msxbiosInitxt:
	PUSH	IX
	LD	IY, (EXPTBL-1)
	LD	IX, INITXT
	CALL	CALSLT
	EI
	POP	IX
	RET
