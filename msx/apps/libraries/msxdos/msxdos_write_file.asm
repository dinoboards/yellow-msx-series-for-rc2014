	include "msxdos.inc"

; extern uint8_t msxdosWriteFile(const uint8_t file_handle, const void *buf, uint16_t nbytes)  __sdcccall(1);
	PUBLIC	_msxdosWriteFile

_msxdosWriteFile:
	LD	IY, 0
	ADD	IY, SP

	LD	B, (IY+2) 	; file_handle
	LD	E, (IY+3)  	; buff
	LD	D, (IY+4)
	LD	L, (IY+5)  	; nbytes
	LD	H, (IY+6)

	ld	c, $49
	CALL	BDOS
	LD	L, A
				; a error code is ignored
	RET
