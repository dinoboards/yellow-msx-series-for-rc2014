

	include "msxdos.inc"

	EXTERN	CDRVR_REGS, CDRVR_REGS_A, CDRVR_REGS_B, CDRVR_REGS_HL

	SECTION	CODE

; extern uint16_t msxdosDrvLunInfo(uint8_t slotNumber, uint8_t driverIndex, uint8_t lunIndex, msxddosLunInfo* pLunInfo);
	PUBLIC	_msxdosDrvLunInfo

_msxdosDrvLunInfo:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	A, (IX+5)
	LD	(CDRVR_REGS_A), A		; driverIndex
	LD	A, (IX+6)
	LD	(CDRVR_REGS_B), A		; lunIndex
	LD	L, (IX+7)  			; pCount
	LD	H, (IX+8)
	LD	(CDRVR_REGS_HL), HL

	LD	A, (IX+4) 			; slotNumber
	LD	C, _CDRVR
	LD	B, 255
	LD	DE, LUN_INFO
	LD	HL, CDRVR_REGS
	CALL	BDOS
	LD	H, A
	PUSH	IX
	POP	AF
	LD	L, A
	POP	IX
	RET
