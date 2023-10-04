	include "msxdos.inc"

_GDLI	EQU	079h

;extern uint8_t msxdosGetDriveLetterInfo(const uint8_t drive_letter, const driveLetterInfo *pInfo);
	PUBLIC	_msxdosGetDriveLetterInfo

_msxdosGetDriveLetterInfo:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	A, (IX+4) 	; drive_leter
	LD	L, (IX+5)  	; DATA pInfo
	LD	H, (IX+6)

	LD	C, _GDLI
	CALL	BDOS
	LD	L, A

	POP	IX
	RET

