

	include "msxdos.inc"

	EXTERN	CDRVR_REGS, CDRVR_REGS_A, CDRVR_REGS_B, CDRVR_REGS_C, CDRVR_REGS_L, CDRVR_REGS_H, CDRVR_REGS_DE, CDRVR_REGS_F

	SECTION	CODE

; extern uint16_t msxdosDevRead(uint8_t driverSlot, uint8_t deviceIndex, uint8_t lunIndex, uint32_t firstDeviceSector, uint8_t sectorCount, uint8_t *buffer);
	PUBLIC	_msxdosDevWrite
_msxdosDevWrite:
	LD	A, 1			; SET FLAG FOR WRITE ACCESS
	LD	(CDRVR_REGS_F), A
	; FALL THRU

	PUBLIC	msxdosDevRw
msxdosDevRw:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	A, (IX+5)
	LD	(CDRVR_REGS_A), A		; driverIndex

	LD	A, (IX+6)
	LD	(CDRVR_REGS_C), A		; lunIndex

	PUSH	IX
	POP	HL
	ADD	HL, 7
	LD	(CDRVR_REGS_DE), HL		; address of firstDeviceSector

	LD	A, (IX+11)			; sector count
	LD	(CDRVR_REGS_B), A

	LD	A, (IX+12)
	LD	(CDRVR_REGS_L), A		; write buffer address
	LD	A, (IX+13)
	LD	(CDRVR_REGS_H), A

	LD	A, (IX+4) 			; slotNumber
	LD	C, _CDRVR
	LD	B, 255
	LD	DE, DEV_RW
	LD	HL, CDRVR_REGS
	CALL	BDOS
	LD	H, A
	PUSH	IX
	POP	AF
	LD	L, A
	POP	IX
	RET


BIOS_RESET:	EQU	0

	PUBLIC	_softReset
_softReset:
	LD	IY, (EXPTBL-1)
	LD	IX, BIOS_RESET
	JP	CALSLT

