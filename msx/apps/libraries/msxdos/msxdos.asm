

	include "msxdos.inc"

	SECTION	DATA
	PUBLIC _HOKVLD, _EXTBIO
	PUBLIC	_msxdosCommand

	SECTION	CODE

; void msxbiosInit32();
	PUBLIC	_msxbiosInit32
_msxbiosInit32:
	PUSH	IX
	LD	IY, (EXPTBL-1)
	LD	IX, INIT32
	CALL	CALSLT
	EI
	POP	IX
	RET


; void msxbiosInitxt();
	PUBLIC	_msxbiosInitxt
_msxbiosInitxt:
	PUSH	IX
	LD	IY, (EXPTBL-1)
	LD	IX, INITXT
	CALL	CALSLT
	EI
	POP	IX
	RET

; void msxbiosInitPalette
	PUBLIC	_msxbiosInitPalette
_msxbiosInitPalette:
	PUSH	IX
	LD	IX, INIPLT
	CALL	CALSUB
	POP	IX
	RET

; void msxbiosPosit(uint16_t col_and_row) __z88dk_fastcall;
	PUBLIC	__msxbiosPosit
__msxbiosPosit:
	PUSH	IX
	LD	IY, (EXPTBL-1)
	LD	IX, POSIT
	CALL	CALSLT
	EI
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

; CALSUB
;
; In: IX = address of routine in MSX2 SUBROM
;     AF, HL, DE, BC = parameters for the routine
;
; Out: AF, HL, DE, BC = depending on the routine
;
; Changes: IX, IY, AF', BC', DE', HL'
;
; Call MSX2 subrom from MSXDOS. Should work with all versions of MSXDOS.
;
; Notice: NMI hook will be changed. This should pose no problem as NMI is
; not supported on the MSX at all.
;
CALSUB:  EXX
         EX     AF, AF'       ; STORE ALL REGISTERS
         LD     HL, EXTROM
         PUSH   HL
         LD     HL, 0C300H
         PUSH   HL           ; PUSH NOP ; JP EXTROM
         PUSH   IX
         LD     HL, 021DDH
         PUSH   HL           ; PUSH LD IX,<ENTRY>
         LD     HL, 03333H
         PUSH   HL           ; PUSH INC SP; INC SP
         LD     HL, 0
         ADD    HL,SP        ; HL = OFFSET OF ROUTINE
         LD     A, 0C3H
         LD     (H_NMI), A
         LD     (H_NMI+1), HL ; JP <ROUTINE> IN NMI HOOK
         EX     AF, AF'
         EXX                 ; RESTORE ALL REGISTERS
         LD     IX, NMI
         LD     IY, (EXPTBL-1)
         CALL   CALSLT      ; CALL NMI-HOOK VIA NMI ENTRY IN ROMBIOS
                             ; NMI-HOOK WILL CALL SUBROM
         EXX
         EX     AF, AF'       ; STORE ALL RETURNED REGISTERS
         LD     HL, 10
         ADD    HL, SP
         LD     SP, HL        ; REMOVE ROUTINE FROM STACK
         EX     AF, AF'
         EXX                 ; RESTORE ALL RETURNED REGISTERS
         RET

BIOS_RESET:	EQU	0

	PUBLIC	_softReset
_softReset:
	LD	IY, (EXPTBL-1)
	LD	IX, BIOS_RESET
	JP	CALSLT

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
