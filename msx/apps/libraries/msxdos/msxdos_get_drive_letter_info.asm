	include "msxdos.inc"

_GDLI	EQU	079h

;extern uint8_t msxdosGetDriveLetterInfo(const uint8_t drive_letter, const driveLetterInfo *pInfo);
	PUBLIC	_msxdosGetDriveLetterInfo

_msxdosGetDriveLetterInfo:
	LD	IY, 0
	ADD	IY, SP

	LD	A, (IY+2) 	; drive_leter
	LD	L, (IY+3)  	; DATA pInfo
	LD	H, (IY+4)

	LD	C, _GDLI
	CALL	BDOS
	LD	L, A

	RET

