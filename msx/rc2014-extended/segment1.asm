
fossil_initialise:
	ld	a, 1				; allocate a system page
	ld	b, 0				; main memory mapper
	CALL	MEMMAP_ALLOC

	jr	NC, allocation_ok

	; what do we do here?
	DI
	HALT

allocation_ok:
	LD	(RS_TMP), a
	push_page_1

	ld	hl, segment1_start
	ld	de, $4000
	ld	bc, segment1_length
	ldir

	LD	HL, RS232_INIT_TABLE
	CALL	RSF_INIT

	LD	HL, SIO_RCVBUF
	LD	C, SIO_BUFSZ
	LD	E, 4
	CALL	RS_OPEN

	pop_page_1

	RET

fossil_set_baud:
	push_page_1

	LD	A, L
	CP	H
	JR	NC, .SKIP1
	LD	A, H				; H IS LARGER, SO USE THAT AS BASIS

.SKIP1:
	CP	5
	JR	NC, .SKIP2			; LESS THAN 5
	LD	A, 5				; SET TO 5
.SKIP2:
	CP 	7
	JR	C, .SKIP3			; GREATER THAN 7
	LD	A, 7				; SET TO 7

.SKIP3:
	LD	HL, FS_RSC_RCV_BAUD
	CP	7
	JR	NZ, .SKIP4
	LD	HL, BAUD_HIGH
	JR	SET_BAUD_EXIT

.SKIP4:
	CP	6
	JR	NZ, SKIP5
	LD	HL, BAUD_MID
	JR	SET_BAUD_EXIT

SKIP5:
	LD	HL, BAUD_LOW

SET_BAUD_EXIT:
	LD	(FS_RSC_RCV_BAUD), HL
	LD	(FS_RSC_SND_BAUD), HL

	ld	c, a				; store the actual selected baud rates
	pop_page_1

	ld	l, c				; return the actual selected baud rates
	ld	h, c
	RET


segment1_start:
	PHASE	$4000

segment1_rs_in:
	LD	D, SIO_BUFSZ/4		; D IS QRT MARK OF BUFFER SIZE
	LD	HL, SIO_RCVBUF
	DI				; AVOID COLLISION WITH INT HANDLER
	LD	A, (RS_DATCNT)		; GET COUNT
	DEC	A			; DECREMENT COUNT
	LD	(RS_DATCNT), A		; SAVE UPDATED COUNT
	CP	D			; BUFFER LOW THRESHOLD
	JR	NZ, SIO_IN1		; IF NOT, BYPASS SETTING RTS

	SIO_CHIP_RTS	CMD_CH, SIO_RTSON
	LD	A, (RS_FLAGS)
	SET	1, A
	LD	(RS_FLAGS), A

SIO_IN1:
	INC	HL
	INC	HL			; HL NOW HAS ADR OF TAIL PTR
	PUSH	HL			; SAVE ADR OF TAIL PTR
	LD	A, (HL)			; DEREFERENCE HL
	INC	HL
	LD	H, (HL)
	LD	L, A			; HL IS NOW ACTUAL TAIL PTR
	LD	C, (HL)			; C := CHAR TO BE RETURNED
	INC	HL			; BUMP TAIL PTR
	POP	DE			; RECOVER ADR OF TAIL PTR
	LD	A, (RS_BUFEND)		; GET BUFEND PTR LOW BYTE
	CP	L			; ARE WE AT BUFF END?
	JR	NZ, SIO_IN2		; IF NOT, BYPASS
	LD	H, D			; SET HL TO
	LD	L, E			; ... TAIL PTR ADR
	INC	HL			; BUMP PAST TAIL PTR
	INC	HL			; ... SO HL NOW HAS ADR OF ACTUAL BUFFER START
SIO_IN2:
	EX	DE, HL			; DE := TAIL PTR VAL, HL := ADR OF TAIL PTR
	LD	(HL), E			; SAVE UPDATED TAIL PTR
	INC	HL
	LD	(HL), D
	EI				; INTERRUPTS OK AGAIN
	RET				; char returned in C

segment1_rs_out:
	LD	B, A				; SAVE CHAR TO BE TRANSMITTED
segment1_rs_out_wait:
	XOR	A				; SELECT READ REGISTER 0
	DI
	OUT	(CMD_CH), A
	IN	A, (CMD_CH)			; GET REGISTER 0 VALUE
	EI
	AND	RS_TRANSMIT_PENDING_MASK	; IS TRANSMIT PENDING?
	JR	Z, segment1_rs_out_wait		; YES, THEN WAIT UNTIL TRANSMIT COMPLETED
	LD	A, B
	SIO_OUT_A				; LOAD BYTE TO TRANSMIT
	OR	$FF				; RETURN NZ TO INDICATE NO TIMEOUT
	RET



RS232_INIT_TABLE:
FS_RSC_CHAR_LEN:	DB	'8'		; Character length '5'-'8'
FS_RSC_PARITY:		DB	'N'		; Parity 'E','O','I','N'
FS_RSC_STOP_BITS:	DB	'1'		; Stop bits '1','2','3'
FS_RSC_XON_XOFF:	DB	'N'		; XON/XOFF controll 'X','N'
FS_RSC_CTR_RTS:		DB	'H'		; CTR-RTS hand shake 'H','N'
FS_RSC_AUTO_RCV_LF:	DB	'N'		; Auto LF for receive 'A','N'
FS_RSC_AUTO_SND_LF:	DB	'N'		; Auto LF for send 'A','N'
FS_RSC_SI_SO_CTRL:	DB	'N'		; SI/SO control 'S','N'
FS_RSC_RCV_BAUD:	DW	19200		; Receiver baud rate  50-19200 ; MARK MAKE BAUD RATE CONFIG
FS_RSC_SND_BAUD:	DW	19200		; Transmitter baud rate 50-19200 ; MARK MAKE BAUD RATE CONFIG
FS_RSC_TIMEOUT_CNT:	DB	0		; Time out counter 0-255

SIO_RCVBUF:
SIO_HD:			DW	SIO_BUF		; BUFFER HEAD POINTER
SIO_TL:			DW	SIO_BUF		; BUFFER TAIL POINTER
SIO_BUF:		DS	SIO_BUFSZ, $00	; RECEIVE RING BUFFER

	DEPHASE
segment1_end:

segment1_length		EQU	segment1_end-segment1_start
