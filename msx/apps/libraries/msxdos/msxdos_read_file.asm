	include "msxdos.inc"

; extern uint16_t msxdosReadFile(const uint8_t file_handle, const void *buf, uint16_t nbytes)  __sdcccall(1);
	PUBLIC	_msxdosReadFile

_msxdosReadFile:
	LD	IY, 0
	ADD	IY, SP

	LD	B, (IY+2) 	; file_handle
	LD	E, (IY+3)  	; buff
	LD	D, (IY+4)
	LD	L, (IY+5)  	; nbytes
	LD	H, (IY+6)

	LD	C, $48
	JP	BDOS
				; a error code is ignored
