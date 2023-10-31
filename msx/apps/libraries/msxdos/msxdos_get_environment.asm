	include "msxdos.inc"

; extern uint8_t msxdosGetEnvironment(const char *name, char *value, const uint8_t buffer_size);
	PUBLIC	_msxdosGetEnvironment

_msxdosGetEnvironment:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+4)		; name
	LD	H, (IX+5)

	LD	E, (IX+6)		; value
	LD	D, (IX+7)

	LD	B, (IX+8)		; size

	LD	C, $6B
	CALL	BDOS
	LD	L, A

	POP	IX
	RET
