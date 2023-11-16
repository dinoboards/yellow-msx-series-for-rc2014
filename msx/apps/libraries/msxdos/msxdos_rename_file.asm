	include "msxdos.inc"

; extern uint8_t msxdosRenameFile(const char * new_filename, const char *filename) __sdcccall(1)
	PUBLIC	_msxdosRenameFile

_msxdosRenameFile:
	LD	C, $4E
	CALL	BDOS
	LD	L, A

	RET
