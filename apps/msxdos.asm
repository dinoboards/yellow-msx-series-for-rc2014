
INITXT	EQU	0x6C
INIT32	EQU	0x6F
POSIT	EQU	0xC6

CALSLT	EQU	0x001C
EXPTBL	EQU	0xFCC1
_EXPLAIN	EQU	0x66
_GDRVR	EQU	0x78
_GPART	EQU	0x7A
_CDRVR	EQU	0x7B
_DIRIO	EQU	0x06
BDOS	EQU	0x0005

; DRIVER FUNCTION ENTRY POINTS
DEV_INFO	EQU 	0x4163
LUN_INFO 	EQU	0x4169

	SECTION	CODE

; void msxbiosInit32();
	PUBLIC	_msxbiosInit32
_msxbiosInit32:
	PUSH	IX
	LD	IY, (EXPTBL-1)
	LD	IX, INIT32
	CALL	CALSLT
	POP	IX
	RET


; void msxbiosInitxt();
	PUBLIC	_msxbiosInitxt
_msxbiosInitxt:
	PUSH	IX
	LD	IY, (EXPTBL-1)
	LD	IX, INITXT
	CALL	CALSLT
	POP	IX
	RET

; void msxbiosPosit(uint8_t col, uint8_t row);
	PUBLIC	_msxbiosPosit
_msxbiosPosit:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	H, (IX+4) 	; COL
	LD	L, (IX+5)  	; ROW

	LD	IY, (EXPTBL-1)
	LD	IX, POSIT
	CALL	CALSLT

	POP	IX
	RET


; extern uint8_t msxdosDirio(uint8_t code) __z88dk_fastcall;
	PUBLIC	_msxdosDirio
_msxdosDirio:
	PUSH	IX

	LD	E, L
	LD	C, _DIRIO
	CALL	BDOS

	POP	IX
	RET


; void msxdosGdrvr(int8_t driverIndex, byte* data) {
	PUBLIC	_msxdosGdrvr
_msxdosGdrvr:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	A, (IX+4) 	; DRIVERINDEX
	LD	L, (IX+5)  	; DATA PTR
	LD	H, (IX+6)

	LD	C, _GDRVR
	CALL	BDOS
	LD	L, A

	POP	IX
	RET

; extern uint8_t  msxdosGpartInfo(uint8_t slotNumber, uint8_t deviceNumber, uint8_t logicalUnitNumber, uint8_t primaryPartitionNumber, uint8_t extendedPartitionNumber, bool getSectorNumber, GPartInfo* result);
	PUBLIC	_msxdosGpartInfo

_msxdosGpartInfo:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+10)
	LD	H, (IX+10)
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

	PUSH	AF
	LD	(GPARTINFO_WRK), BC
	LD	(GPARTINFO_WRK+2), DE
	LD	(GPARTINFO_WRK+4), HL
	LD	(GPARTINFO_WRK+6), IY
	LD	(GPARTINFO_WRK+8), IX

	POP	DE
	LD	HL, GPARTINFO_WRK
	LD	BC, 10
	LDIR

	POP	AF
	LD	L, A

	POP	IX
	RET

; extern uint8_t msxdosExplain(uint8_t code, char* buffer);

	PUBLIC	_msxdosExplain

_msxdosExplain:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	B, (IX+4)
	LD	E, (IX+5)
	LD	D, (IX+6)

	LD	C, _EXPLAIN
	CALL	BDOS

	POP	IX
	RET

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

; extern uint16_t msxdosDrvDevGetName(uint8_t slotNumber, uint8_t driverIndex, char* pDeviceName);
	PUBLIC	_msxdosDrvDevGetName

_msxdosDrvDevGetName:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	A, (IX+5)
	LD	(CDRVR_REGS_A), A		; driverIndex
	LD	L, (IX+6)  			; pCount
	LD	H, (IX+7)
	LD	(CDRVR_REGS_HL), HL
	LD	A, 2
	LD	(CDRVR_REGS_B), A		; REQUEST DRIVER NAME

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
	RET

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

GPARTINFO_WRK:
CDRVR_REGS:
CDRVR_REGS_F:	DB	0
CDRVR_REGS_A:	DB	0
CDRVR_REGS_BC:
CDRVR_REGS_C:	DB	0
CDRVR_REGS_B:	DB	0
CDRVR_REGS_DE:
CDRVR_REGS_E:	DB	0
CDRVR_REGS_D:	DB	0
CDRVR_REGS_HL:
CDRVR_REGS_L:	DB	0
CDRVR_REGS_H:	DB	0
