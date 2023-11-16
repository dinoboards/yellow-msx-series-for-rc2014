	include "msxdos.inc"

; extern uint8_t msxdosGetEnvironment(const char *name, char *value, const uint8_t buffer_size);
	PUBLIC	_msxdosGetEnvironment

_msxdosGetEnvironment:
	LD	IY, 0
	ADD	IY, SP

	LD	L, (IY+2)		; name
	LD	H, (IY+3)

	LD	E, (IY+4)		; value
	LD	D, (IY+5)

	LD	B, (IY+6)		; size

	LD	C, $6B
	CALL	BDOS
	LD	L, A

	RET
