	include "msxdos.inc"

; extern uint8_t msxdosFindEnvironment(const uint16_t index, char *env_item, uint8_t buffer_size)
	PUBLIC	_msxdosFindEnvironment

_msxdosFindEnvironment:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	E, (IX+4) 	; index
	LD	D, (IX+5) 	; index
	LD	L, (IX+6) 	; env_item
	LD	H, (IX+7) 	; env_item
	LD	B, (IX+8) 	; buffer_size

	LD	C, $6D
	CALL	BDOS
	LD	L, A

	POP	IX
	RET
