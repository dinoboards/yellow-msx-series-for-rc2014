	include "msxdos.inc"

;extern void msxdosGetTime(uint8_t *hour, uint8_t *minutes, uint8_t *seconds);
	PUBLIC	_msxdosGetTime

_msxdosGetTime:
	LD	C, $2C
	CALL	BDOS

	LD	IY, 0
	ADD	IY, SP

	LD	C, L
	LD	B, H

	LD	L, (IY+2)		; HOUR POINTER
	LD	H, (IY+3)
	LD	(HL), B

	LD	L, (IY+4)		; MIN POINTER
	LD	H, (IY+5)
	LD	(HL), C

	LD	L, (IY+6)		; SEC POINTER
	LD	H, (IY+7)
	LD	(HL), D

	RET

