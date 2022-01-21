


fossil_initialise:
	ld	a, (RS_P1_SEG)
	or	a				; have we already allocated a segment?
	jr	nz, allocation_ok		; yes - so skip allocation request

	ld	a, 1				; allocate a system page
	ld	b, 0				; main memory mapper
	CALL	MEMMAP_ALLOC
	jr	NC, allocation_ok

	; what do we do here?
	DI
	HALT

allocation_ok:
	LD	(RS_P1_SEG), a

	push_page_1	0

	ld	hl, segment1_start
	ld	de, $4000
	ld	bc, segment1_length
	ldir

	CALL	SIO_INIT
	XOR	A
	LD	(RS_FLAGS), A

	; INSTALL INTERRUPT HOOK - IF NOT ALREADY DONE

	LD	DE, RS_OLDINT		; COPY HOOK FUNCTION FROM H_KEYI TO RS_OLDINT
	LD	A, (DE)
	OR	A
	JR	NZ, fossil_init_buffer
					; install interrupt handler

	LD	HL, H_KEYI
	LD	BC, 5
	LDIR				; COPY THE OLD INT HOOK

	; INSTALL INTERRUPT HOOK TO SIO_INT
	LD	A, $C3                  ; JUMP
	LD	DE, (WORK)
	LD	HL, @SIO_INT             ; SET TO JUMP TO SIO_INIT IN PAGE 3 RAM
	ADD	HL, DE

	LD	(H_KEYI), A             ; SET JMP INSTRUCTION
	LD	(H_KEYI+1), HL          ; SET NEW INTERRUPT ENTRY POINT

fossil_init_buffer:
	XOR	A
	LD	(RS_DATCNT), A
	LD	(RS_FLAGS), A

	LD	HL, SIO_BUF
	LD	(SIO_HD), HL
	LD	(SIO_TL), HL

	LD      HL, RS_FLAGS
	SET     3, (HL)			; SET RS232 OPEN FLAG
	SET	1, (HL)			; SET RTS ON FLAG
	LD	A, (RS_SIO_B_CMD)
	LD	C, A
	SIO_CHIP_RTS	(C), SIO_RTSON

	pop_page_1	0
	EI
	RET

fossil_deinit:
	XOR	A				; MARK AS CLOSED AND RTS OFF
	LD	(RS_FLAGS), A
	LD	A, (RS_SIO_B_CMD)
	LD	C, A
	SIO_CHIP_RTS	(C), SIO_RTSOFF

	LD	A, (RS_P1_SEG)
	LD	B, 0				; main memory mapper
	JP	MEMMAP_FREE

fossil_set_baud:
	ld	a, l
	cp	h
	jr	nc, .skip1
	ld	l, h					; h is larger than l, so use that

.skip1:
	ld	a, (RS_SIO_CLK)
	ld	d, a
	ld	ix, baud_mapping_table
	ld	bc, baud_clock_map_size

.loop
	ld	a, (ix+baud_clock_map.sio_clk_code)

	cp	d					; is this entry for the sio/2 clock?
	jr	z, .found_entry

.try_next
	add	ix, bc
	jr	.loop

.found_entry:
	ld	a, (ix+baud_clock_map.fossil_max_code)
	cp	l					; is it <= to fossil_max_code
	jr	c, .try_next 				; no, try next record

	; found our entry

	ld	a, (ix+baud_clock_map.sio_clk_divider)
	ld	(RS_SIO_CLK_DIV), a
	ld	c, (ix+baud_clock_map.fossil_sel_code)	; get the baud rate selected

	ld	l, c					; return the actual selected baud rate
	ld	h, c
	RET


	STRUCT baud_clock_map
sio_clk_code	BYTE
sio_clk_divider	BYTE
fossil_max_code	BYTE
fossil_sel_code	BYTE
	ENDS

	; Map SIO/2 clock rate code to fossil baud rate code
baud_mapping_table:
	baud_clock_map	{ SIO_CLK_307200, SIO_CLK_DIV_64, FOSSIL_BAUD_4800, FOSSIL_BAUD_4800  }
baud_clock_map_size	EQU	$-baud_mapping_table
	baud_clock_map	{ SIO_CLK_307200, SIO_CLK_DIV_32, FOSSIL_BAUD_9600,  FOSSIL_BAUD_9600  }
	baud_clock_map	{ SIO_CLK_307200, SIO_CLK_DIV_16, $FF,               FOSSIL_BAUD_19200 }
	baud_clock_map	{ SIO_CLK_614400, SIO_CLK_DIV_64, FOSSIL_BAUD_9600,  FOSSIL_BAUD_9600  }
	baud_clock_map	{ SIO_CLK_614400, SIO_CLK_DIV_32, FOSSIL_BAUD_19200, FOSSIL_BAUD_19200  }
	baud_clock_map	{ SIO_CLK_614400, SIO_CLK_DIV_16, $FF,		     FOSSIL_BAUD_38400  }

segment1_start:
	PHASE	$4000

	; NEED TO ALIGN TO PAGE
SIO_RCVBUF:
SIO_BUF:		DS	SIO_BUFSZ, $00	; RECEIVE RING BUFFER
SIO_BUFEND:
SIO_HD:			DW	SIO_BUF		; BUFFER HEAD POINTER
SIO_TL:			DW	SIO_BUF		; BUFFER TAIL POINTER

segment1_rs_in:
	DI						; ATOMIC DEC (RS_DATCNT)
	LD	A, (RS_DATCNT)				; GET COUNT
	DEC	A					; DECREMENT COUNT
	LD	(RS_DATCNT), A				; SAVE UPDATED COUNT
	EI
	CP	SIO_BUFSZ/4				; BUFFER LOW THRESHOLD
	JR	Z, segment1_rs_in_set_rts		; YES, SET RTS

SIO_IN1:
	LD	HL, (SIO_TL)
	LD	C, (HL)					; C := CHAR TO BE RETURNED
	INC	L					; BUMP TAIL PTR
	LD	(SIO_TL), HL				; SAVE UPDATED TAIL PTR
	RET						; CHAR RETURNED IN C

segment1_rs_in_set_rts:
	DI
	LD	A, (RS_SIO_B_CMD)
	LD	C, A
	SIO_CHIP_RTS	(C), SIO_RTSON
	LD	HL, RS_FLAGS
	SET 	1, (HL)
	EI
	JP	SIO_IN1

segment1_rs_out:
	LD	H, A				; SAVE CHAR TO BE TRANSMITTED
	LD	BC, (RS_SIO_B_CMD)		; L => CMD, H => DAT PORTS
	XOR	A				; SELECT READ REGISTER 0
	DI
	OUT	(C), A
segment1_rs_out_wait:
	IN	A, (C)				; GET REGISTER 0 VALUE
	AND	RS_TRANSMIT_PENDING_MASK	; IS TRANSMIT PENDING?
	JR	Z, segment1_rs_out_wait		; YES, THEN WAIT UNTIL TRANSMIT COMPLETED
	LD	C, B				; LOAD DAT PORT INTO C
	OUT	(C), H				; TRANSMIT BYTE
	EI
	OR	$FF				; RETURN NZ TO INDICATE NO TIMEOUT
	RET


segment1_sio_interrupt:
	EXX
	LD	HL, RS_DATCNT		; buffer size ptr
	LD	D, SIO_BUFSZ-10		; buffer high mark
	EXX

	LD	DE, (RS_SIO_B_CMD)	; E => CMD, D => DAT

	LD	HL, RS_FLAGS		; IS OPENED?
	BIT	3, (HL)			; FLAG PORT OPEN?
	JR	Z, SIO_INT_ABORT

	LD	HL, (SIO_HD)		; HL IS HEAD - ADDR OF NEXT BYTE TO BE WRITTEN IN BUFFER

	; RECEIVE CHARACTER INTO BUFFER
SIO_INTRCV1:
	LD	C, D			; DAT PORT
	IN	B, (C)			; READ PORT

	EXX
	LD	A, (HL)			; GET COUNT
	INC	A			; INCREMENT THE COUNT
	JR	Z, SIO_INTRCV4		; BAIL OUT IF BUFFER FULL, RCV BYTE DISCARDED

	LD	(HL), A			; AND SAVE IT
	CP	D			; HIT HIGH WATER MARK?
	EXX
	JR	NZ, SIO_INTRCV2		; IF NOT, BYPASS CLEARING RTS

	LD	C, E			; CMD PORT
	SIO_CHIP_RTS	(C), SIO_RTSOFF
	LD	A, (RS_FLAGS)		; SET BIT FLAG FOR RTS OFF
	AND	11111101B		; RES	1, A
	LD	(RS_FLAGS), A

SIO_INTRCV2:
	LD	(HL), B			; SAVE CHARACTER RECEIVED IN BUFFER AT HEAD
	INC	L			; BUMP HEAD POINTER

SIO_INTRCV3:
	; CHECK FOR MORE PENDING...
	XOR	A
	LD	C, E			; CMD PORT
	OUT	(C), A			; READ REGISTER 0
	IN	A, (C)
	RRA				; READY BIT TO CF
	JP	C, SIO_INTRCV1		; IF SET, DO SOME MORE

	LD	A, (RS_FLAGS)
	AND	00000010B		; BIT 1, A
	JR	Z, SIO_UPDATE_HEAD_PTR	; ABORT NOW IF RTS IS OFF

	; TEST FOR NEW BYTES FOR A SHORT PERIOD OF TIME
	LD	B, 95
SIO_MORE:
	IN	A, (C)			; C IS CMD PORT
	RRA				; READY BIT TO CF
	JR	C, SIO_INTRCV1		; IF SET, DO SOME MORE
	DJNZ	SIO_MORE

COMMAND_RETURN_FROM_INT	EQU	0x38

SIO_UPDATE_HEAD_PTR:
	LD	(SIO_HD), HL

SIO_INTRCV4:
	; LATCH OF SIO/2 INTERRUPT STATE
	XOR	A
	OUT	(SIO0A_CMD), A
	LD	A, COMMAND_RETURN_FROM_INT
	OUT	(SIO0A_CMD), A
	RET

SIO_INT_ABORT:
	LD	C, D			; DAT PORT
	IN	A, (C)
	LD	C, E			; CMD PORT
	; PORT NOT OPENED, SO IGNORE BYTE, RESET RTS AND EXIT
	SIO_CHIP_RTS	(C), SIO_RTSOFF
	RES	1, (HL)			; SET BIT FLAG FOR RTS OFF
	JR	SIO_INTRCV4


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

	DEPHASE
segment1_end:

segment1_length		EQU	segment1_end-segment1_start
