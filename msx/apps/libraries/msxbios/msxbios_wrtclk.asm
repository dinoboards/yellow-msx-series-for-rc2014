

	include "msx.inc"

	SECTION	CODE
	EXTERN  CALSUB

; void msxbiosWrtClk(const uint8_t reg, const uint8_t value) __sdcccall(1);
	PUBLIC	_msxbiosWrtClk
_msxbiosWrtClk:
	PUSH	IX
	LD	IX, WRTCLK

	LD	C, A	; reg -> C
	LD	A, L	; value -> A

	CALL	CALSUB
	EI
	POP	IX
	RET

