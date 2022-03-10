

	ORG	$4000
	DS	$100
	
	; IMAGE FOR SECONDARY BANKS (INCLUDED WITH ROM SECTOR DATA)

	EXPORT	SEC_RAW_CPY
	EXPORT	SECSTRT
	EXPORT	SECEND
	EXPORT	DATSIZE

	; ORG WILL BE $4100

SEC_RAW_CPY:
	; C IS THE RELATIVE OFFSET WITHIN LOCAL SECTORS
	CALL	MY_STWORK

	CALL	SECTOR_ADDR_IN_BANK
	CALL	COPY_SECTOR_TO_USR
	RET

SECTOR_ADDR_IN_BANK:
	LD	HL, SECSTRT		; AN ARRAY OF 512 SECTORS PER DRIVER BANK
	LD	A, C			; RELATIVE OFFSET
	RLCA				; BC = A * 512
	LD	C, 0
	LD	B, A
	ADD	HL, BC
	RET

	; HL MUST POINT TO SOURCE SECTOR DATA
COPY_SECTOR_TO_USR:
	; dbg.break

	LD	E, (IX+ST_WRKAREA.DEST_LW)
	LD	D, (IX+ST_WRKAREA.DEST_HI)
	LD	BC, 512
	; HL IS SECTOR SOUND ADDR
	; DE IS DESTINATION ADDR
	LDIR	; COPY TO USER DATA PAGE
	LD	(IX+ST_WRKAREA.DEST_HI), D
	RET

;-----------------------------------------------------------------------------
;
; Obtain the work area address for the driver
; Input: A=1  to obtain the work area for the master, 2 for the slave
; Preserves A

MY_STWORK:
	PUSH	AF
	XOR	A
	EX	AF,AF'
	XOR	A
	LD	IX, GWORK
	CALL	CALBNK
	push	hl
	ld	l, (ix)
	ld	h, (ix+1)
	push	hl
	pop	ix
	pop	hl
	POP	AF
	RET

; 4000h   +---------------------+
;         |     Page 0 code     |
;         +---------------------+
; 40FEh   |          K          |
;         +---------------------+
; 40FFh   |       Bank ID       |
; 4100h   +---------------------+
;         |     driver code     |
; SECSTRT +---------------------+
;         |                     |
;         |    sector data 	|
;         |                     |
; 7FD0h   +---------------------+
;         | Bank switching code |
; 7FFFh   +---------------------+

SECTOR_INDX_START	EQU	07F00h
SECTORS			EQU	05F00h		; 8k of data (16 sectors)

DRV_END:

SECSTRT:
	DS	SECCNT * SECSIZ, 0		; reserve space for embedded disk image
SECEND:
DATSIZE	EQU	SECCNT * SECSIZ
	END
