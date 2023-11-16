

	include "msxdos.inc"

	EXTERN	CDRVR_REGS_A, CDRVR_REGS_B, CDRVR_REGS_HL, CDRVR_REGS
	SECTION	CODE

; extern uint16_t _msxdosDrvDevLogicalUnitCount(uint8_t slotNumber, uint8_t driverIndex, uint8_t* pCount);
	PUBLIC	_msxdosDrvDevLogicalUnitCount
_msxdosDrvDevLogicalUnitCount:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	A, (IX+5)
	LD	(CDRVR_REGS_A), A		; driverIndex
	LD	L, (IX+6)  			; pCount
	LD	H, (IX+7)
	LD	(CDRVR_REGS_HL), HL
	XOR	A
	LD	(CDRVR_REGS_B), A		; REQUEST BASIC INFO (LUN COUNT)

	LD	A, (IX+4) 			; slotNumber
	LD	C, _CDRVR
	LD	B, 255
	LD	DE, DEV_INFO
	LD	HL, CDRVR_REGS
	CALL	BDOS
	LD	H, A
	PUSH	IX
	POP	AF
	LD	L, A
	POP	IX
	RET
