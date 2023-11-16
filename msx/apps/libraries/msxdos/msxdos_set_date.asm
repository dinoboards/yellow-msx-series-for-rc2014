	include "msxdos.inc"

; extern uint8_t msxdosSetDate(uint16_t year, uint8_t month, uint8_t date);
	PUBLIC	_msxdosSetDate

_msxdosSetDate:
	LD	IY, 0
	ADD	IY, SP

	LD	L, (IY+2)		; YEAR
	LD	H, (IY+3)
	LD	D, (IY+4)		; MONTH
	LD	E, (IY+5)		; DATE

	ld	c, $2B
	CALL	BDOS
	LD	L, A

	RET

