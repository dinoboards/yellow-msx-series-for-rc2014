	include "msxdos.inc"

; extern uint8_t msxdosSetDate(uint16_t year, uint8_t month, uint8_t date);
	PUBLIC	_msxdosSetDate

_msxdosSetDate:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+4)		; YEAR
	LD	H, (IX+5)
	LD	D, (IX+6)		; MONTH
	LD	E, (IX+7)		; DATE

	ld	c, $2B
	CALL	BDOS
	LD	L, A

	POP	IX
	RET

