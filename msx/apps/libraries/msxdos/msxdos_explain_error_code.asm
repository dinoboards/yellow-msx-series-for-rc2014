	include "msxdos.inc"

; extern void msxdosExplainErrorCode(const uint8_t error_code, char * const error_description);
	PUBLIC	_msxdosExplainErrorCode

_msxdosExplainErrorCode:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	B, (IX+4)		; error_code

	LD	E, (IX+5)		; error_description_buffer
	LD	D, (IX+6)

	LD	C, $66
	CALL	BDOS

	POP	IX
	RET
