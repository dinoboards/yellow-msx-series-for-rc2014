

	include "msx.inc"

	SECTION	CODE
	EXTERN  CALSUB

; void msxdbiosInitPalette
	PUBLIC	_msxdbiosInitPalette
_msxdbiosInitPalette:
	PUSH	IX
	LD	IX, INIPLT
	CALL	CALSUB
	POP	IX
	RET
