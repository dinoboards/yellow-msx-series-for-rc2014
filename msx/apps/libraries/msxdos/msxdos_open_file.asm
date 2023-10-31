	include "msxdos.inc"

; extern uint8_t msxdosOpenFile(const char *filename, const uint8_t open_mode);
	PUBLIC	_msxdosOpenFile

_msxdosOpenFile:
	LD	IY, 0
	ADD	IY, SP

	LD	E, (IY+2)
	LD	D, (IY+3)
	LD	A, (IY+4)

	LD	C, $43
	CALL	BDOS
	LD	L, B
	LD	H, A

	RET
