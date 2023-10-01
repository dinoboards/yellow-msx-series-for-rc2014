; COMPACT FLASH/IDE DRIVER
; ONLY WORKS FOR 8 BIT IDE MODE
; TODO: ERROR HANDLING


CFBASE	EQU	010h		; IDE 0: IO BASE ADDRESS


;The addresses that the CF Card resides in I/O space.
;Change to suit hardware.
CFDATA	EQU	CFBASE + 00		; Data (R/W)
CFERR	EQU	CFBASE + 01		; Error register (R)
CFFEAT	EQU	CFBASE + 01		; Features (W)
CFSECCO	EQU	CFBASE + 02		; Sector count (R/W)
CFLBA0	EQU	CFBASE + 03		; LBA bits 0-7 (R/W, LBA mode)
CFLBA1	EQU	CFBASE + 04		; LBA bits 8-15 (R/W, LBA mode)
CFLBA2	EQU	CFBASE + 05		; LBA bits 16-23 (R/W, LBA mode)
CFLBA3	EQU	CFBASE + 06		; LBA bits 24-27 (R/W, LBA mode)
CFSTAT	EQU	CFBASE + 07		; Status (R)
CFCMD	EQU	CFBASE + 07		; Command (W)

FEATURE_8_BIT	EQU	1
ATA_CMD_SET_FEAT	EQU	0EFh

IDE_IDNT_MODEL_NUMBER	EQU	036H		; MODEL NUMBER (40 CHARS LONG)
IDE_IDNT_SECTOR_COUNT	EQU	114		; 32 UNSIGNED INT

WAIT_OUTER	EQU	0
WAIT_INNER	EQU	100

LD_IXSP	MACRO
	LD	IX, 0
	ADD	IX, SP
	ENDM

AL_HLSP	MACRO	BYTE_COUNT
	LD	HL, 0-BYTE_COUNT
	ADD	HL, SP
	LD	SP, HL
	ENDM

LD_HLSP	MACRO	OFFSET
	LD	HL, OFFSET
	ADD	HL, SP
	ENDM

LD_HLIX	MACRO	OFFSET
	PUSH	IX
	POP	HL
	ADD	HL, OFFSET
	ENDM

; RETURNS Z IF NOT PRESENT
; RETURNS NZ IF PRESENT

DRV_INIT_CF:
	CALL	CF_PROBE
	JR	Z, CF_FOUND

CF_NOT_FOUND:
	XOR	A
	RET

CF_FOUND:
	CALL	CF_INIT
	JR	NZ, CF_NOT_FOUND

	LD	HL, CF_FOUND_MSG
	CALL	PRINT

	PUSH	BC
	PUSH	DE

	LD_IXSP
	AL_HLSP	512
	CALL	CF_READ_IDENT
	LD_HLSP	+IDE_IDNT_SECTOR_COUNT
	LD	SP, IX

	PUSH	HL
	POP	IX

	; IX IS PTR TO 4 BYTE SECTOR COUNT

	LD	C, (IX+2)
	LD	B, (IX+3)
	PUSH	BC
	LD	C, (IX+0)
	LD	B, (IX+1)
	PUSH	BC
	CALL	_convert_and_print_disk_size
	POP	HL
	POP	HL

	LD	HL, MSG.STORAGE_LINE_END
	CALL	PRINT

	POP	DE
	POP	BC


	OR	255
	RET


; Read or write logical sectors from/to a logical unit
;
;Input:    Cy=0 to read, 1 to write
;          A = Device number, 1 to 7
;          B = Number of sectors to read or write
;          C = Logical unit number, 1 to 7
;          HL = Source or destination memory address for the transfer
;          DE = Address where the 4 byte sector number is stored.
;Output:   A = Error code (the same codes of MSX-DOS are used):
;              0: Ok
;              .IDEVL: Invalid device or LUN
;              .NRDY: Not ready
;              .DISK: General unknown disk error
;              .DATA: CRC error when reading
;              .RNF: Sector not found
;              .UFORM: Unformatted disk
;              .WPROT: Write protected media, or read-only logical unit
;              .WRERR: Write error
;              .NCOMP: Incompatible disk.
;              .SEEK: Seek error.
;          B = Number of sectors actually read (in case of error only)
DEV_READ_CF:
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_RW_CF_ERR
	LD 	A, B			; STORE SECTOR COUNT
	LD	C, A

	OUT 	(CFSECCO), A
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_RW_CF_ERR

	LD	A, (DE)
	INC	DE
	OUT	(CFLBA0), A			;LBA 0:7
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_RW_CF_ERR

	LD	A, (DE)
	INC	DE
	OUT	(CFLBA1),A			;LBA 8:15
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_RW_CF_ERR

	LD	A, (DE)
	INC	DE
	OUT 	(CFLBA2),A			;LBA 16:23
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_RW_CF_ERR

	LD	A, (DE)
	AND	00FH			; LBA 24:27 + DRV 0 selected + bits 5:7=111
	OR	0E0H			; SELECT MASTER DRIVE
	OUT 	(CFLBA3), A
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_RW_CF_ERR

	PUSH	BC			; SAVE SECTOR COUNT (C)
	CALL	CF_RD_CMD
	POP	BC			; RESTORE SECTOR COUNT
	JR	NZ, DEV_RW_CF_ERR

	; RETURN Z/NZ OF CF_RD_CMD
	; TODO RETURN CORRECT B VALUE FOR ERR CONDITION
	LD	B, C
	RET

DEV_RW_CF_ERR:
	LD	B, 0
	RET


DEV_WRT_CF:
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_RW_CF_ERR
	LD 	A, B			; STORE SECTOR COUNT
	LD	C, A

	OUT 	(CFSECCO), A
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_RW_CF_ERR

	LD	A, (DE)
	INC	DE
	OUT	(CFLBA0), A			;LBA 0:7
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_RW_CF_ERR

	LD	A, (DE)
	INC	DE
	OUT	(CFLBA1),A			;LBA 8:15
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_RW_CF_ERR

	LD	A, (DE)
	INC	DE
	OUT 	(CFLBA2),A			;LBA 16:23
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_RW_CF_ERR

	LD	A, (DE)
	AND	00FH			; LBA 24:27 + DRV 0 selected + bits 5:7=111
	OR	0E0H			; SELECT MASTER DRIVE
	OUT 	(CFLBA3), A
	CALL 	CF_WAIT_BUSY
	JR	NZ, DEV_RW_CF_ERR

	PUSH	BC			; SAVE SECTOR COUNT (C)
	CALL	CF_WR_CMD
	POP	BC			; RESTORE SECTOR COUNT
	JR	NZ, DEV_RW_CF_ERR

	; RETURN Z/NZ OF CF_RD_CMD
	; TODO RETURN CORRECT B VALUE FOR ERR CONDITION
	LD	B, C
	RET

DEV_INFO_CF_FLDS:
	PUSH	IX
	LD_IXSP

	EX	DE, HL		; PLACE DESTINATION PTR INTO DE
	EX	AF, AF'		; SAVE A

	AL_HLSP	512
	CALL	CF_READ_IDENT

	EX	AF, AF'		; RESTORE A
	; HL REFERENCES MODEL NUMBER
	; DE IS RETURN BUFFER PTR

	CP	1
	JR	Z, DEV_INFO_CF_MAN

	CP	2
	JR	Z, DEV_INFO_CF_NAME

	CP	3
	JR	Z, DEV_INFO_CF_SERIAL

	LD	A, 1

DEV_INFO_CF_END:
	; RESTORE SP
	LD	SP, IX
	POP	IX
	RET

DEV_INFO_CF_MAN:
	LD_HLSP	+IDE_IDNT_MODEL_NUMBER
	LD	BC, 40
	LDIR
	LD	B, 64-40
	CALL	RIGHT_PAD
	JR	DEV_INFO_CF_END

DEV_INFO_CF_NAME:
	LD	HL, CF_DRV_NAME
	LD	BC, CF_DRV_NAME_LEN
	LDIR
	LD	B, 64-CF_DRV_NAME_LEN
	CALL	RIGHT_PAD
	JR	DEV_INFO_CF_END

DEV_INFO_CF_SERIAL:
	; LD_HLSP	+IDE_IDNT_SERIAL_NUMBER

	EX	DE, HL
	LD	BC, 63
	ADD	HL, BC

	LD	(HL), '1'
	DEC	HL
	LD	(HL), '2'
	DEC	HL
	CALL	LEFT_PAD
	JR	DEV_INFO_CF_END


LUN_INFO_CF:
	LD	A, B
	CP	1
	JR	NZ, LUN_INFO_CF_INVALID

	PUSH	IX
	EX	DE, HL
	LD_IXSP
	AL_HLSP	512
	CALL	CF_READ_IDENT

	LD_HLSP	+IDE_IDNT_SECTOR_COUNT
	EX	DE, HL
	LD	SP, IX			; RESTORE SP
	POP	IX

	; HL IS PTR TO RETURN BUFFER
	; DE IS PTR TO SECTOR COUNT

	LD	A, 0			; standard block device
	LD	(HL), 0
	INC	HL

	LD	(HL), 0			; sector size 512
	INC	HL
	LD	(HL), 2
	INC	HL

	EX	DE, HL			; COPY SECTOR COUNT 32BIT VALUE
	LD	BC, 4
	LDIR
	EX	DE, HL

	LD	(HL), 0			; writable
	INC	HL

	LD	(HL), 0			; cyclinders
	INC	HL
	LD	(HL), 0
	INC	HL

	LD	(HL), 0			; heads
	INC	HL

	LD	(HL), 0			; sectors per track
	XOR	A

	RET

LUN_INFO_CF_INVALID:
	LD	A, 1
	RET

CF_PROBE:
	LD	A, 1
	OUT	(CFSECCO), A
	LD	A, 2
	OUT	(CFLBA0), A
	LD	A, 4
	OUT	(CFLBA1), A

	IN	A, (CFSECCO)
	CP	1
	RET	NZ

	IN	A, (CFLBA0)
	CP	2
	RET	NZ

	IN	A, (CFLBA1)
	CP	4
	RET

CF_INIT:
	CALL	CF_WAIT_BUSY
	RET	NZ
	LD	A, FEATURE_8_BIT			; ENABLE 8 BIT MODE FEATURE
	OUT	(CFFEAT), A
	CALL	CF_WAIT_BUSY
	RET	NZ
	LD	A, ATA_CMD_SET_FEAT			; SEND FEATURE COMMAND
	OUT	(CFCMD), A

	; FALL THRU TO CF_WAIT_BUSY

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
	JR	Z, CF_WAIT_BUSY9			; RETURN A == 0, Z

	DEC	DE
	LD	A, D
	OR	E
	JR	NZ, CF_WAIT_BUSY2
	DJNZ	CF_WAIT_BUSY1

	XOR	A				; RETURN A=.DISK, NZ
	OR	ERR.DISK

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
	OR	ERR.DISK

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
	IN	A,(CFSTAT)
	BIT	0, A
	JR	NZ, CF_WAIT_DAT_RDY_ERR				; RETURN ERROR status
	AND	088H					; %10001000 mask off busy and drq bits
	XOR	008H					; %00001000 we want busy(7) to be 0 and drq(3) to be 1
	JR	Z, CF_WAIT_DAT_RDY9

	DEC	DE
	LD	A, D
	OR	E
	JR	NZ, CF_WAIT_DAT_RDY2
	DJNZ	CF_WAIT_DAT_RDY1

	XOR	A
	OR	ERR.DISK

CF_WAIT_DAT_RDY9:
	POP	DE
	POP	BC
	RET

CF_WAIT_DAT_RDY_ERR:
	POP	DE
	POP	BC

CF_ERROR:			; TODO TRANSLATE TO CORRECT MSX-DOS ERROR CODES
	IN	A, (CFERR)
	OR	A
	RET


CF_READ_IDENT:
	CALL	CF_WAIT_CMD_RDY
	JR	NZ, CF_READ_IDENT_ERR

	LD	A, 0E0H					; SELECT MASTER DRIVE
	OUT 	(CFLBA3),A					;

	LD	A, 0ECH					; identity command
	OUT	(CFCMD), A

	LD	B, 0					; DELAY
loop:	djnz	loop
	; //TODO CHECK ERROR
	LD	C, 1					; READ 1 SECTOR
	CALL	CF_REDDAT
	JR	NZ, CF_READ_IDENT_ERR

	XOR	A
	RET

CF_READ_IDENT_ERR:
	RET




;***************************************************************************
;CF_RD_CMD
;Function: Gets a sector (512 bytes) into RAM buffer.
; number of sectors to read in B
; destorys B and C
;***************************************************************************
CF_RD_CMD:
	CALL	CF_WAIT_CMD_RDY
	JR	NZ, CF_RW_CMD_ERR		;Make sure drive is ready for command

	LD	A, 020H				;Prepare read command
	OUT	(CFCMD), A				;Send read command
	CALL	CF_WAIT_DAT_RDY			;Wait until data is ready to be read
	JR	NZ, CF_RW_CMD_ERR		;Make sure drive is ready for command

	IN	A, (CFSTAT)				;Read status
	AND	01H				;mask off error bit
	JR	NZ, CF_ERROR			;return NZ if error

	LD	C, B				; number of sectors to read
CF_REDDAT:						; READ sector into HL
	LD 	B, 0				; read 256 words (512 bytes per sector)
CF_RD_SECT:
	CALL	CF_WAIT_DAT_RDY
	JR	NZ, CF_RW_CMD_ERR		;Make sure drive is ready for command

	IN 	A, (CFDATA)				; get byte of ide data
	LD 	(HL), A
	INC 	HL
	CALL	CF_WAIT_DAT_RDY
	JR	NZ, CF_RW_CMD_ERR		;Make sure drive is ready for command

	IN 	A, (CFDATA)				; get byte of ide data
	LD 	(HL), A
	INC 	HL
	DJNZ 	CF_RD_SECT

	DEC	C
	JR	NZ, CF_REDDAT

	XOR	A
	RET

CF_RW_CMD_ERR:
	LD	B, 0
	RET

;***************************************************************************
;CF_WR_CMD
;Function: Writes a sector (512 bytes) from RAM buffer.
; number of sectors to read in B
; destorys B and C
;***************************************************************************
CF_WR_CMD:
	CALL	CF_WAIT_CMD_RDY		;Make sure drive is ready for command
	JR	NZ, CF_RW_CMD_ERR		;Make sure drive is ready for command

	LD	A, 030H			;Prepare write command
	OUT	(CFCMD), A			;Send read command
	CALL	CF_WAIT_DAT_RDY		;Wait until data is ready to be read
	JR	NZ, CF_RW_CMD_ERR		;Make sure drive is ready for command

	IN	A, (CFSTAT)			;Read status
	AND	01H			;mask off error bit
	JR	NZ, CF_ERROR		;return if error

	LD	C, B
CF_WRTDAT:
	LD 	B, 0			;write 256 words (512 bytes per sector)
CF_WR_SECT:
	CALL	CF_WAIT_DAT_RDY
	JR	NZ, CF_RW_CMD_ERR		;Make sure drive is ready for command

	LD	A, (HL)
	OUT 	(CFDATA), A			;set byte of ide data
	INC 	HL
	CALL	CF_WAIT_DAT_RDY
	JR	NZ, CF_RW_CMD_ERR		;Make sure drive is ready for command

	LD	A, (HL)
	OUT 	(CFDATA), A			;set byte of ide data
	INC 	HL
	DJNZ 	CF_WR_SECT

	DEC	C
	JR	NZ, CF_WRTDAT

	XOR	A
	RET


CF_FOUND_MSG:
	DB	"COMPACT FLASH:   (", 0

CF_DRV_NAME:
	DB	"IDE/CompactFlash"

CF_DRV_NAME_LEN	EQU	$-CF_DRV_NAME
