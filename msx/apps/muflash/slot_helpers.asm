
DEFC SSL_REGS		= 0xFFFF
DEFC PSL_STAT		= 0xA8
DEFC MSX_MUSIC_PAGE 	= 0xE000
DEFC ROM_05555H		= (0x5555)
DEFC ROM_02AAAH		= (0x2AAA+0x4000)

	SECTION CODE_COMPILER

; SWITCH MSX-MUSIC ROM PAGE TO PAGE L
; (SWITCH PAGE 3 TO SLOT 3-1 TO ENABLE MSX-MUSIC PAGING ADDRESS)
;
; MSX-MUSIC'S ROM PAGE CAN BE SELECTED BY WRITING TO THE STANDARD MEMORY MAPPED WRITE REGISTER IN PAGE 1 OR PAGE 2
; THIS FUNCTION USES PAGE 3, TO AVOID INTERFERING WITH THE SST39SF040 WRITE COMMAND SEQUENCE
;
; L -> DESIRED MSX-MUSIC ROM PAGE
;
	PUBLIC	_msxMusicSetPage
_msxMusicSetPage:
MSX_MUSIC_SET_PAGE:
	; THIS FUNCTION CAN NOT USE THE STACK WHILE PAGE 3 IS SWITCHED OUT

	IN	A, (PSL_STAT)
	LD	D, A			; STORE CURRENT SLOTS ASSIGNMENTS IN D
	OR	A, 0b11000000		; MASK PAGE 3 TO SLOT 3
	OUT	(PSL_STAT), A		; APPLY SLOT ASSIGNMENTS

	LD	A, (SSL_REGS)
	CPL
	LD	E, A			; STORE CURRENT SUB-SLOTS ASSIGNMENTS IN E
	AND	A, 0b00111111		; MASK OUT PAGE 3 SUB-SLOT
	OR	A, 0b01000000		; SET PAGE 3 TO SUB-SLOT 1
	LD	(SSL_REGS), A		; APPLY SUB-SLOT ASSIGNMENTS

	LD	A, L
	LD	(MSX_MUSIC_PAGE), A	; SET MSX-MUSIC PAGE TO DESIRED PAGE

	LD	A, E			; RESTORE SUB-SLOT ASSIGNMENTS
	LD	(SSL_REGS), A

	LD	A, D			; RESTORE SLOT ASSIGNMENTS
	OUT	(PSL_STAT), A
	RET
;
; SET PAGE 1 TO MSX-MUSIC SLOT (3-1)
;
; Output: H -> original slot, L -> original sub slot
;
MSX_MUSIC_SET_SLOT:
	IN	A, (PSL_STAT)
	LD	H, A			; STORE CURRENT SLOTS ASSIGNMENTS IN D
	OR	A, 0b11001100		; MASK SLOT 3 FOR PAGE 1 AND 3
	OUT	(PSL_STAT), A		; APPLY SLOT ASSIGNMENTS
	LD	D, A

	LD	A, (SSL_REGS)
	CPL
	LD	L, A			; STORE CURRENT SUB-SLOTS ASSIGNMENTS IN L
	AND	A, 0b11110011		; MASK OUT PAGE 1 SUB-SLOT
	OR	A, 0b00000100		; SET PAGE 1 TO SUB-SLOT 1
	LD	E, A
	LD	(SSL_REGS), A		; APPLY SUB-SLOT ASSIGNMENTS

	LD	A, H			; RETRIEVE ORIGINAL SLOT ASSIGNMENTS
	OR	A, 0b00001100		; KEEP PAGE 1 TO SLOT 3
	OUT	(PSL_STAT), A		; APPLY SLOT ASSIGNMENTS

					; H -> ORIGINAL SLOT, L -> ORIGINAL SUB-SLOT
	RET

;
; RESTORE SLOT/SUB-SLOT ASSIGNMENTS
; H -> SLOT ASSIGNEMTNS
; L -> SUB SLOT ASSIGNMENTS
;
	PUBLIC	_msxMusicRestoreSlot
_msxMusicRestoreSlot:
MSX_MUSIC_RESTORE_SLOT:
	IN	A, (PSL_STAT)
	OR	A, 0b11000000		; MASK PAGE 3 TO SLOT 3
	OUT	(PSL_STAT), A		; APPLY SLOT ASSIGNMENTS

	LD	A, L			; LOAD ORIGINAL SUB-SLOT ASSIGNMENTS
	LD	(SSL_REGS), A		; RESTORE ORIGINAL SUB-SLOT ASSIGNMENTS

	LD	A, H			; LOAD ORIGINAL SLOT ASSIGNMENTS
	OUT	(PSL_STAT), A		; RESTORE ORIGINAL SLOT MAPPPINGS

	RET

MSX_MUSIC_BYTE_PROGRAM:
	LD	L, 1				; 5555H=AAH
	CALL	MSX_MUSIC_SET_PAGE
	LD	A, 0xAA
	LD	(ROM_05555H), A

	LD	L, 0				; 2AAAH=55H
	CALL	MSX_MUSIC_SET_PAGE
	LD	A, 0x55
	LD	(ROM_02AAAH), A

	LD	L, 1				; 5555H=A0H
	CALL	MSX_MUSIC_SET_PAGE
	LD	A, 0xA0
	LD	(ROM_05555H), A

	RET

	PUBLIC	_msx_music_erase_ROM
;
; void msx_music_erase_ROM()
;
; APPLY THE COMMAND SEQUENCE TO ERASE THE ROM
; 5555H=AAH, 2AAAH=55H, 5555H=80H, 5555H=AAH, 2AAAH=55H, 5555H=10H
_msx_music_erase_ROM:
	DI

	ld b,b
	jr $+2

	CALL	MSX_MUSIC_SET_SLOT
	LD	(current_page_slots), HL

	LD	L, 1				; 5555H=AAH
	CALL	MSX_MUSIC_SET_PAGE
	LD	A, 0xAA
	LD	(ROM_05555H), A

	LD	L, 0				; 2AAAH=55H
	CALL	MSX_MUSIC_SET_PAGE
	LD	A, 0x55
	LD	(ROM_02AAAH), A

	LD	L, 1				; 5555H=80H
	CALL	MSX_MUSIC_SET_PAGE
	LD	A, 0x80
	LD	(ROM_05555H), A

	LD	A, 0xAA				; 5555H=AAH
	LD	(ROM_05555H), A

	LD	L, 0				; 2AAAH=55H
	CALL	MSX_MUSIC_SET_PAGE
	LD	A, 0x55
	LD	(ROM_02AAAH), A

	LD	L, 1				; 5555H=10H
	CALL	MSX_MUSIC_SET_PAGE
	LD	A, 0x10
	LD	(ROM_05555H), A

	LD	L, 0				;
	CALL	MSX_MUSIC_SET_PAGE

WAIT_FOR_ERASE:
	LD	A, (0x4000)
	AND	0x80
	JR	Z, WAIT_FOR_ERASE

	LD	HL, (current_page_slots)
	CALL	MSX_MUSIC_RESTORE_SLOT
	EI
	RET

	PUBLIC	_msx_music_write_4K_page
;
; void msx_music_write_4K_page(uint8_t page, uint8_t buffer[4096])
;
; WRITE sequence byte program
;  5555H=AAH, 2AAAH=55H, 5555H=A0H, ADDR=Data

_msx_music_write_4K_page:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	DI				; do we need this?

	CALL	MSX_MUSIC_SET_SLOT
	LD	(current_page_slots), HL

	LD	A, (IX+4) 		; page
	AND	3			; calculate 4K address start within the MSX Slot Page 1 (0x4000-0x7FFF)
	ADD	4
	RLCA
	RLCA
	RLCA
	RLCA
	LD	D, A
	LD	E, 0			; DE will be 0x4000, 0x5000, 0x6000 or 0x7000

	LD	L, (IX+5)  		; buffer (source addres)
	LD	H, (IX+6)		; into HL
	LD	BC, 0x1000		; loop counter - 4K
LOOPWR:
	PUSH	HL
	PUSH	DE
	PUSH	BC

	CALL	MSX_MUSIC_BYTE_PROGRAM

	LD	A, (IX+4) 		; page
	SRL	A			; divide by 4
	SRL	A
	LD	L, A
	CALL	MSX_MUSIC_SET_PAGE	; address the required page

	POP	BC			; restore counter
	POP	DE			; restore source and destination
	POP	HL

	LD	A, (HL)			; copy data and increment
	LD	(DE), A
	INC	HL
	INC	DE

	DEC	BC
	LD	A, B
	OR	C
	JR	NZ, LOOPWR

	LD	L, 0				; restore page of MSX-MUSIC rom
	CALL	MSX_MUSIC_SET_PAGE

	LD	HL, (current_page_slots)
	CALL	MSX_MUSIC_RESTORE_SLOT

	EI
	POP	IX
	RET

; extern bool     msx_music_verify_4K_page(uint8_t page, uint8_t buffer[4096]);  // Page is 0 to 127 - to address a 4K block within the 512K ROM address range.

	PUBLIC	_msx_music_verify_4K_page

_msx_music_verify_4K_page:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	DI				; do we need this?

	CALL	MSX_MUSIC_SET_SLOT
	LD	(current_page_slots), HL

	LD	A, (IX+4) 		; page
	AND	3			; calculate 4K address start within the MSX Slot Page 1 (0x4000-0x7FFF)
	ADD	4
	RLCA
	RLCA
	RLCA
	RLCA
	LD	D, A
	LD	E, 0			; DE will be 0x4000, 0x5000, 0x6000 or 0x7000

	LD	L, (IX+5)  		; buffer (source addres)
	LD	H, (IX+6)		; into HL
	LD	BC, 0x1000		; loop counter - 4K
LOOPRD:
	PUSH	HL
	PUSH	DE
	PUSH	BC

	LD	A, (IX+4) 		; page
	SRL	A			; divide by 4
	SRL	A
	LD	L, A
	CALL	MSX_MUSIC_SET_PAGE	; address the required page

	POP	BC			; restore counter
	POP	DE			; restore source and destination
	POP	HL

	LD	A, (DE)			; compare data and increment
	CP	(HL)			; same?
	JR	NZ, VERIFY_FAIL		; jump to abort
	INC	HL
	INC	DE

	DEC	BC
	LD	A, B
	OR	C
	JR	NZ, LOOPRD

	LD	L, 0				; restore page of MSX-MUSIC rom
	CALL	MSX_MUSIC_SET_PAGE

	LD	HL, (current_page_slots)
	CALL	MSX_MUSIC_RESTORE_SLOT

	LD	HL, 1				; return TRUE
	EI
	POP	IX
	RET

VERIFY_FAIL:
	LD	L, 0				; restore page of MSX-MUSIC rom
	CALL	MSX_MUSIC_SET_PAGE

	LD	HL, (current_page_slots)
	CALL	MSX_MUSIC_RESTORE_SLOT

	LD	HL, 0				; return FALSE
	EI
	POP	IX
	RET

	SECTION bss_compiler

current_page_slots:	DW	0
