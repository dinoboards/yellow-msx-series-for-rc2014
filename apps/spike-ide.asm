
	PUBLIC	_cfInit, _cfReadTest, _cfWriteTest, _cfReadIdentity, _cfProbe

	SECTION	CODE

_NCOMP	equ	0FFh
_WRERR	equ	0FEh
_DISK	equ	0FDh
_NRDY	equ	0FCh
_DATA	equ	0FAh
_RNF	equ	0F9h
_WPROT	equ	0F8h
_UFORM	equ	0F7h
_SEEK	equ	0F3h
_IFORM	equ	0F0h
_IDEVL	equ	0B5h
_IPARM	equ	08Bh

CFBASE	EQU	$10		; IDE 0: IO BASE ADDRESS

WAIT_OUTER	EQU	0
WAIT_INNER	EQU	4000

;The addresses that the CF Card resides in I/O space.
;Change to suit hardware.
CFDATA:	EQU	CFBASE + $00		; Data (R/W)
CFERR:	EQU	CFBASE + $01		; Error register (R)
CFFEAT:	EQU	CFBASE + $01		; Features (W)
CFSECCO:	EQU	CFBASE + $02		; Sector count (R/W)
CFLBA0:	EQU	CFBASE + $03		; LBA bits 0-7 (R/W, LBA mode)
CFLBA1:	EQU	CFBASE + $04		; LBA bits 8-15 (R/W, LBA mode)
CFLBA2:	EQU	CFBASE + $05		; LBA bits 16-23 (R/W, LBA mode)
CFLBA3:	EQU	CFBASE + $06		; LBA bits 24-27 (R/W, LBA mode)
CFSTAT:	EQU	CFBASE + $07		; Status (R)
CFCMD:	EQU	CFBASE + $07		; Command (W)

;***************************************************************************
;CF_INIT
;Function: Initialize CF to 8 bit data transfer mode
;***************************************************************************
_cfInit:
	CALL	CF_WAIT_BUSY
	JR	NZ, _cfInitErr
	LD	A,$01						;LD features register to enable 8 bit
	OUT	(CFFEAT),A
	CALL	CF_WAIT_BUSY
	JR	NZ, _cfInitErr
	LD	A,$EF						;Send set features command
	OUT	(CFCMD),A
	CALL	CF_WAIT_BUSY
	JR	NZ, _cfInitErr

	LD	L, 0
	RET

_cfInitErr:
	LD	L, 255
	RET


;***************************************************************************
; CF_WAIT_BUSY
; FUNCTION: LOOPS UNTIL STATUS REGISTER BIT 7 (BUSY) IS 0
;***************************************************************************

CF_WAIT_BUSY:
	PUSH	BC
	PUSH	DE
	LD	B, WAIT_OUTER

CF_WAIT_BUSY1:
	LD	DE, WAIT_INNER

CF_WAIT_BUSY2:
	IN	A, (CFSTAT)				; READ STATUS
	AND	080h				; MASK BUSY BIT
	JR	Z, CF_WAIT_BUSY9

	DEC	DE
	LD	A, D
	OR	E
	JR	NZ, CF_WAIT_BUSY2
	DJNZ	CF_WAIT_BUSY1

	XOR	A
	OR	_DISK

CF_WAIT_BUSY9:
	POP	DE
	POP	BC
	RET

;***************************************************************************
;CF_WAIT_CMD_RDY
;Function: Loops until status register bit 7 (busy) is 0 and drvrdy(6) is 1
;***************************************************************************
CF_WAIT_CMD_RDY:
	PUSH	BC
	PUSH	DE
	LD	B, WAIT_OUTER

CF_WAIT_CMD_RDY1:
	LD	DE, WAIT_INNER

CF_WAIT_CMD_RDY2:
	IN	A, (CFSTAT)					;Read status
	AND	0C0H					; %11000000 mask off busy and rdy bits
	XOR	040H					; %01000000 we want busy(7) to be 0 and drvrdy(6) to be 1
	JR	Z, CF_WAIT_CMD_RDY9

	DEC	DE
	LD	A, D
	OR	E
	JR	NZ, CF_WAIT_CMD_RDY2
	DJNZ	CF_WAIT_CMD_RDY1

	XOR	A
	OR	_DISK

CF_WAIT_CMD_RDY9:
	POP	DE
	POP	BC
	RET
;***************************************************************************
;CF_WAIT_DAT_RDY
;Function: Loops until status register bit 7 (busy) is 0 and drq(3) is 1
;***************************************************************************
CF_WAIT_DAT_RDY:
	PUSH	BC
	PUSH	DE
	LD	B, WAIT_OUTER

CF_WAIT_DAT_RDY1:
	LD	DE, WAIT_INNER

CF_WAIT_DAT_RDY2:
	IN	A,(CFSTAT)					;Read status
	AND	088H					; %10001000 mask off busy and drq bits
	XOR	008H					; %00001000 we want busy(7) to be 0 and drq(3) to be 1
	JR	Z, CF_WAIT_DAT_RDY9

	DEC	DE
	LD	A, D
	OR	E
	JR	NZ, CF_WAIT_DAT_RDY2
	DJNZ	CF_WAIT_DAT_RDY1

	XOR	A
	OR	_DISK

CF_WAIT_DAT_RDY9:
	POP	DE
	POP	BC
	RET

;***************************************************************************
;CF_RD_CMD
;Function: Gets a sector (512 bytes) into RAM buffer.
;***************************************************************************
CF_RD_CMD:
	CALL	CF_WAIT_CMD_RDY
	RET	NZ				;Make sure drive is ready for command
	LD	A, $20					;Prepare read command
	OUT	(CFCMD), A				;Send read command
	CALL	CF_WAIT_DAT_RDY				;Wait until data is ready to be read
	RET	NZ

	IN	A, (CFSTAT)				;Read status
	AND	%00000001				;mask off error bit
	RET	NZ				; RETURN NZ if error

CF_REDDAT:						; READ sector into HL
	LD 	B, 0				; read 256 words (512 bytes per sector)
CF_RD_SECT:
	CALL	CF_WAIT_DAT_RDY
	RET	NZ
	IN 	A, (CFDATA)				;get byte of ide data
	LD 	(HL), A
	INC 	HL
	CALL	CF_WAIT_DAT_RDY
	RET	NZ
	IN 	A, (CFDATA)				;get byte of ide data
	LD 	(HL), A
	INC 	HL
	DJNZ 	CF_RD_SECT

	XOR	A
	RET


;***************************************************************************
;CF_TEST
;Function: Read a sector into RAM buffer.
;***************************************************************************

; extern void cfReadTest(ioBuf*)
_cfReadTest:
DEV_READ_CF:
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_READ_CF_ERR

	LD 	A, 1
	OUT 	(CFSECCO), A					; Deal with only one sector at a time (512 bytes)
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_READ_CF_ERR

	LD	A, (HL)
	INC	HL
	OUT	(CFLBA0), A					;LBA 0:7
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_READ_CF_ERR

	LD	A, (HL)
	INC	HL
	OUT	(CFLBA1),A					;LBA 8:15
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_READ_CF_ERR

	LD	A, (HL)
	INC	HL
	OUT 	(CFLBA2),A					;LBA 16:23
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_READ_CF_ERR

	LD	A, (HL)
	INC	HL
	AND	$0F						; LBA 24:27 + DRV 0 selected + bits 5:7=111
	OR	$E0						; SELECT MASTER DRIVE
	OUT 	(CFLBA3), A
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_READ_CF_ERR

	PUSH	BC			; SAVE SECTOR COUNT (C)
	CALL	CF_RD_CMD
	POP	BC			; RESTORE SECTOR COUNT
	JR	NZ, DEV_READ_CF_ERR

	LD	L, 0
	RET

DEV_READ_CF_ERR:
	LD	L, 255
	RET


_cfReadIdentity:
	CALL	CF_WAIT_CMD_RDY
	JR	NZ, _cfReadIdentityErr
	LD	A, $E0						; SELECT MASTER DRIVE
	OUT 	(CFLBA3),A					;

	LD	A, $EC						; identity command
	OUT	(CFCMD), A

	LD	B, 0
loop:
	djnz	loop

	; //TODO CHECK ERROR

	CALL	CF_REDDAT
	JR	NZ, _cfReadIdentityErr

	LD	L, 0
	RET

_cfReadIdentityErr:
	LD	L, 255
	RET

CF_WR_CMD:
	CALL	CF_WAIT_CMD_RDY				;Make sure drive is ready for command
	LD	A, $30					;Prepare write command
	OUT	(CFCMD), A				;Send read command
	CALL	CF_WAIT_DAT_RDY				;Wait until data is ready to be read

	IN	A, (CFSTAT)				;Read status
	AND	%00000001				;mask off error bit
	RET	NZ				;return if error

	LD 	B, 0					;write 256 words (512 bytes per sector)
CF_WR_SECT:
	CALL	CF_WAIT_DAT_RDY
	LD	A, (HL)
	OUT 	(CFDATA), A				;set byte of ide data
	INC 	HL
	CALL	CF_WAIT_DAT_RDY
	LD	A, (HL)
	OUT 	(CFDATA), A				;set byte of ide data
	INC 	HL
	DJNZ 	CF_WR_SECT
	RET

_cfWriteTest:
	CALL	CF_WAIT_CMD_RDY
	JR	NZ, _cfWriteTestErr

	LD 	A, 1
	OUT 	(CFSECCO), A					; Deal with only one sector at a time (512 bytes)
	CALL 	CF_WAIT_BUSY
	JR	NZ, _cfWriteTestErr

	LD	A, (HL)
	INC	HL
	OUT	(CFLBA0), A					;LBA 0:7
	CALL 	CF_WAIT_BUSY
	JR	NZ, _cfWriteTestErr

	LD	A, (HL)
	INC	HL
	OUT	(CFLBA1),A					;LBA 8:15
	CALL 	CF_WAIT_BUSY
	JR	NZ, _cfWriteTestErr

	LD	A, (HL)
	INC	HL
	OUT 	(CFLBA2),A					;LBA 16:23
	CALL 	CF_WAIT_BUSY
	JR	NZ, _cfWriteTestErr

	LD	A, (HL)
	INC	HL
	AND	$0F						; LBA 24:27 + DRV 0 selected + bits 5:7=111
	OR	$E0						; SELECT MASTER DRIVE
	OUT 	(CFLBA3), A
	CALL 	CF_WAIT_BUSY
	JR	NZ, _cfWriteTestErr

	CALL	CF_WR_CMD
	JR	NZ, _cfWriteTestErr

	LD	L, 0
	RET

_cfWriteTestErr:
	LD	L, 255
	RET

_cfProbe:
	LD	A, 1
	OUT	(CFSECCO), A
	LD	A, 2
	OUT	(CFLBA0), A
	LD	A, 4
	OUT	(CFLBA1), A

	IN	A, (CFSECCO)
	CP	1
	JR	NZ, NO_CF

	IN	A, (CFLBA0)
	CP	2
	JR	NZ, NO_CF

	IN	A, (CFLBA1)
	CP	4
	JR	NZ, NO_CF

	LD	L, 255
	RET

NO_CF:
	LD	L, 0
	RET
