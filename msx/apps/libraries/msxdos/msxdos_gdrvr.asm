
	include "msxdos.inc"

	SECTION	CODE


; void msxdosGdrvr(int8_t driverIndex, byte* data) {
	PUBLIC	_msxdosGdrvr
_msxdosGdrvr:
	LD	IY, 0
	ADD	IY, SP

	LD	A, (IY+2) 	; DRIVERINDEX
	LD	L, (IY+3)  	; DATA PTR
	LD	H, (IY+4)

	LD	C, _GDRVR
	CALL	BDOS
	LD	L, A

	RET
