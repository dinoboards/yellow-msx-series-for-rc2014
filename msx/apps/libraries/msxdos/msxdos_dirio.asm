
	include "msxdos.inc"

	SECTION	CODE

; extern uint8_t msxdosDirio(uint8_t code) __z88dk_fastcall;
	PUBLIC	_msxdosDirio
_msxdosDirio:
	LD	E, L
	LD	C, _DIRIO
	JP	BDOS

