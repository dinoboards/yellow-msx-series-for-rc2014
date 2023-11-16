	include "msxdos.inc"

; extern void msxdosExplainErrorCode(const uint8_t error_code, char * const error_description);
	PUBLIC	_msxdosExplainErrorCode

_msxdosExplainErrorCode:
	LD	IY, 0
	ADD	IY, SP

	LD	B, (IY+2)		; error_code

	LD	E, (IY+3)		; error_description_buffer
	LD	D, (IY+4)

	LD	C, $66
	JP	BDOS
