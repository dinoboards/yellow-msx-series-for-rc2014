
	PUBLIC	_cRawIo, _cRead
	SECTION code_user

C_READ:		EQU	1
C_RAWIO:	EQU	6
BDOS:		EQU	5

_cRawIo:
	LD	C, C_RAWIO
	LD	E, $FF
	CALL	BDOS
	LD	L, A
	RET

_cRead:
	LD	C, C_READ
	JP	BDOS
