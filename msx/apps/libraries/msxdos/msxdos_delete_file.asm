	include "msxdos.inc"

; extern uint8_t msxdosDeleteFile(const char * new_filename) __sdcccall(1)
	PUBLIC	_msxdosDeleteFile

_msxdosDeleteFile:
	EX	DE, HL
	LD	C, $4D
	CALL	BDOS
	LD	L, A

	RET
