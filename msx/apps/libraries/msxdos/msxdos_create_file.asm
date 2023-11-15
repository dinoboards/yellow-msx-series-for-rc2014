	include "msxdos.inc"

; extern uint8_t msxdosCreateFile(const char *filename, const uint8_t open_mode)
	PUBLIC	_msxdosCreateFile

_msxdosCreateFile:
	LD	IY, 0
	ADD	IY, SP

	LD	E, (IY+2)
	LD	D, (IY+3)
	LD	A, (IY+4)
	LD	B, 0

	LD	C, $44
	CALL	BDOS
	LD	L, B
	LD	H, A

	RET
