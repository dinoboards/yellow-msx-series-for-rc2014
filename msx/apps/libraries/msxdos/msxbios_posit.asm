

	include "msx.inc"

	SECTION	CODE

; void msxbiosPosit(uint16_t col_and_row) __z88dk_fastcall;
	PUBLIC	__msxbiosPosit
__msxbiosPosit:
	PUSH	IX
	LD	IY, (EXPTBL-1)
	LD	IX, POSIT
	CALL	CALSLT
	EI
	POP	IX
	RET
