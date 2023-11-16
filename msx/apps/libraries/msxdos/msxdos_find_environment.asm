	include "msxdos.inc"

; extern uint8_t msxdosFindEnvironment(const uint16_t index, char *env_item, uint8_t buffer_size)
	PUBLIC	_msxdosFindEnvironment

_msxdosFindEnvironment:
	LD	IY, 0
	ADD	IY, SP

	LD	E, (IY+2) 	; index
	LD	D, (IY+3) 	; index
	LD	L, (IY+4) 	; env_item
	LD	H, (IY+5) 	; env_item
	LD	B, (IY+6) 	; buffer_size

	LD	C, $6D
	CALL	BDOS
	LD	L, A

	RET
