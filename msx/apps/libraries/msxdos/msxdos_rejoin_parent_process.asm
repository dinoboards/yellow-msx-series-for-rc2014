	include "msxdos.inc"

; extern uint16_t msxdosRejoinParentProcess(const uint8_t parent_process_id) __z88dk_fastcall;
	PUBLIC	_msxdosRejoinParentProcess

_msxdosRejoinParentProcess:
	PUSH	IX
	LD	B, L

	LD	C, $61
	CALL	BDOS
	LD	L, A
	LD	H, B
	POP	IX
	RET
