	include "msxdos.inc"

; extern uint16_t msxdosCloseFile(const uint8_t file_handle) __sdcccall(1);
	PUBLIC	_msxdosCloseFile

_msxdosCloseFile:
	PUSH	IX
	LD	B, A

	LD	C, $45
	CALL	BDOS
	LD	L, A
	LD	H, 0

	POP	IX
	RET
