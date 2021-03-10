
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
DEV_RW	EQU	0x4160

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

; extern uint8_t  msxdosGpart(uint8_t slotNumber, uint8_t deviceNumber, uint8_t logicalUnitNumber, uint8_t primaryPartitionNumber, uint8_t extendedPartitionNumber, bool getSectorNumber, GPartInfo* result);
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
	LD	(GPARTINFO_WRK+3), IX
	LD	(GPARTINFO_WRK+5), IY
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


; extern uint16_t msxdosDevRead(uint8_t driverSlot, uint8_t deviceIndex, uint8_t lunIndex, uint32_t firstDeviceSector, uint8_t sectorCount, uint8_t *buffer);
	PUBLIC	_msxdosDevRead
_msxdosDevRead:
	XOR	A			; SET FLAG FOR READ ACCESS
	LD	(CDRVR_REGS_F), A
	JR	msxdosDevRw

; extern uint16_t msxdosDevRead(uint8_t driverSlot, uint8_t deviceIndex, uint8_t lunIndex, uint32_t firstDeviceSector, uint8_t sectorCount, uint8_t *buffer);
	PUBLIC	_msxdosDevWrite
_msxdosDevWrite:
	LD	A, 1			; SET FLAG FOR WRITE ACCESS
	LD	(CDRVR_REGS_F), A
	; FALL THRU

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

	SECTION	BSS
	PUBLIC	_workingMsxDosBuff

_workingMsxDosBuff:
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
