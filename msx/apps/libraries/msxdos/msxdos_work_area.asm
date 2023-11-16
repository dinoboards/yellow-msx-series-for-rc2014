

	include "msxdos.inc"

	EXTERN	GPARTINFO_WRK
	SECTION	CODE

; extern uint8_t  msxdosGpart(uint8_t slotNumber, uint8_t deviceNumber, uint8_t logicalUnitNumber, uint8_t primaryPartitionNumber, uint8_t extendedPartitionNumber, bool getSectorNumber, GPartInfo* result);
;                                     IX+4              IX+5                     ix+6                         ix+7                               ix+8                      ix+9               ix+10/11
	PUBLIC	_msxdosGpart

_msxdosGpart:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+10)
	LD	H, (IX+11)
	PUSH	HL		; SAVE RESULT PTR

	LD	H, (IX+7)		; PART NUMBER OR (IX+9)
	LD	A, (IX+9)
	OR	A
	JR	Z, GPARTINFO1
	SET	7, H

GPARTINFO1:
	LD	A, (IX+4)		; SLOT NUMBER
	LD	B, 0xFF
	LD	D, (IX+5)		; DEVICE NUMBER
	LD	E, (IX+6)		; LUN
	LD	L, (IX+8)		; EXTENDED PART NUMBER

	LD	C, _GPART
	CALL	BDOS

	LD	(GPARTINFO_WRK+0), A
	LD	(GPARTINFO_WRK+1), BC
	LD	(GPARTINFO_WRK+3), IY
	LD	(GPARTINFO_WRK+5), IX
	LD	(GPARTINFO_WRK+7), DE
	LD	(GPARTINFO_WRK+9), HL

	POP	DE
	LD	HL, GPARTINFO_WRK+1
	LD	BC, 10
	LDIR

	LD	A, (GPARTINFO_WRK+0)
	LD	L, A

	POP	IX
	RET
