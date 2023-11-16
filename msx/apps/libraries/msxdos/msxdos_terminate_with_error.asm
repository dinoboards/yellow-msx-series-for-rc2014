	include "msxdos.inc"

; extern uint16_t msxdosTerminateWithError(const uint8_t error) __z88dk_fastcall;
	PUBLIC	_msxdosTerminateWithError

_msxdosTerminateWithError:
	LD	B, L
	LD	C, $62
	JP	BDOS
