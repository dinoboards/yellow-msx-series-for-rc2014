	include "msxdos.inc"

; extern uint8_t msxdosSetTime(uint8_t hour, uint8_t minutes, uint8_t seconds, uint8_t centiseconds);
	PUBLIC	_msxdosSetTime

_msxdosSetTime:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	H, (IX+4)		; HOUR
	LD	L, (IX+5)		; MINUTES
	LD	D, (IX+6)		; SECONDS
	LD	E, (IX+7)		; CENTI-SECONDS

	ld	c, $2D
	CALL	BDOS
	LD	L, A

	POP	IX
	RET

