	include "msxdos.inc"

;extern void msxdosGetTime(uint8_t *hour, uint8_t *minutes, uint8_t *seconds);
	PUBLIC	_msxdosGetTime

_msxdosGetTime:
	PUSH	IX

	LD	C, $2C
	CALL	BDOS

	LD	IX, 0
	ADD	IX, SP

	LD	C, L
	LD	B, H

	LD	L, (IX+4)		; HOUR POINTER
	LD	H, (IX+5)
	LD	(HL), B

	LD	L, (IX+6)		; MIN POINTER
	LD	H, (IX+7)
	LD	(HL), C

	LD	L, (IX+8)		; SEC POINTER
	LD	H, (IX+9)
	LD	(HL), D

	POP	IX
	RET

