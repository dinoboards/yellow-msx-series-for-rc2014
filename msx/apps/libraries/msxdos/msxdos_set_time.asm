	include "msxdos.inc"

; extern uint8_t msxdosSetTime(uint8_t hour, uint8_t minutes, uint8_t seconds, uint8_t centiseconds);
	PUBLIC	_msxdosSetTime

_msxdosSetTime:
	LD	IY, 0
	ADD	IY, SP

	LD	H, (IY+2)		; HOUR
	LD	L, (IY+3)		; MINUTES
	LD	D, (IY+4)		; SECONDS
	LD	E, (IY+5)		; CENTI-SECONDS

	ld	c, $2D
	CALL	BDOS
	LD	L, A

	RET

