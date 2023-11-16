

	include "msx.inc"

	SECTION	CODE

; void msxbiosInit32();
	PUBLIC	_msxbiosInit32
_msxbiosInit32:
	PUSH	IX
	LD	IY, (EXPTBL-1)
	LD	IX, INIT32
	CALL	CALSLT
	EI
	POP	IX
	RET

