
; TODO
; [ ] INIT FUNCTION SHOULD ATTEMPT TO SET STOP BITS/PARITY/ETC, SPEED
; [ ] OPEN - ERROR UNLESS RAW MODE, ERROR IF ALREADY OPENED, ENABLE RTS
; [ ] STAT - IMPLEMENT
; [ ] SNDCHR XON/XOFF FLOW CONTROL
; [x] CLOSE - DISABLE RTS
; [ ] EOF - ALWAYS FALSE
; [ ] LOF - IMPLEMENT
; [ ] SNDBRK - RESEARCH AND IMPLEMENT
; [ ] DTR - IMPLEMENT
; [ ] SETCHN - IMPLEMENT (SIO PORTS)

EXTBIO_RS2_JUMP_TABLE:
	DEFB	0		; MSX serial features (no TxReady INT, No Sync detect, No Timer INT, No CD, No RI)
	DEFB	1		; version number
	DEFB	0		; reserved for future expansion
	JP	RS_INIT		; initialize RS232C port
	JP	RS_OPEN		; open RS232C port
	JP	STAT		; ReaD STATus
	JP	GETCHR		; reveive data
	JP	RS_SNDCHR		; send data
	JP	CLOSE		; close RS232C port
	JP	EOF		; tell EOF code received
	JP	LOC		; reports number of characters in the receiver buffer
	JP	LOF		; reports number of free space left in the receiver buffer
	JP	BACKUP		; back up a character
	JP	SNDBRK		; send break character
	JP	DTR		; turn on/off DTR line
	JP	SETCHN		; set channel number


;------------------------------------
	; Entry:  [HL]= address of the parameter table
	;	 [B] = slot address of the parameter table
	; Return: carry flag is set if illegal parameters are contained
	; Modify: [AF]

	PUBLIC	RS_INIT
RS_INIT:				; initialize RS232C port
	DI

	XOR	A
	LD	(RS_FLAGS), A

	SIO_CHIP_INIT	SIO0A_CMD, SIO_RTSOFF
	SIO_CHIP_INIT	SIO0B_CMD, SIO_RTSOFF
	EI
	XOR	A
	RET

	PUBLIC	RS_OPEN
; ------------------------------------------------
RS_OPEN:				; open RS232C port
	EI
	XOR	A
	LD	(RS_DATCNT), A
	LD	(RS_FLAGS), A

	LD	(RS_FCB), HL
	LD      A, C
	LD      (RS_IQLN),A

	LD	D, H		; FIRST 2 WORDS OF BUFFER AT THE HEAD AND TAIL PTRS
	LD	E, L		; THEY NEED TO BE INITIALISED TO START OF ACTUAL DATA BUFFER
	EX	DE, HL		; WHICH IS JUST AFTER THESE 4 BYTES
	INC	DE
	INC	DE
	INC	DE
	INC	DE
	LD	(HL), E		; LOAD FIRST 2 WORDS IN BUFFER TO POINT TO ADDRESS
	INC	HL		; AFTER FIRST 2 WORDS
	LD	(HL), D
	INC	HL
	LD	(HL), E
	INC	HL
	LD	(HL), D
	INC	HL

	EX	DE, HL
	LD	B, 0
	ADD	HL, BC
	LD	(RS_BUFEND), HL

	LD      HL, RS_FLAGS
	SET     3, (HL)			; SET RS232 OPEN FLAG
	SET	1, (HL)			; SET RTS ON FLAG
	SIO_CHIP_RTS	CMD_CH, SIO_RTSON
	RET

STAT:				; Read Status
	RET

GETCHR:				; reveive data
	CALL	SIO_RCBBYT
	OR	A		; CLEAR CARRY FLAG (NEVER HAVE EOF)
	RET

	PUBLIC	RS_SNDCHR
RS_TRANSMIT_PENDING_MASK	EQU	$40	; BIT 2 OF SIO REGISTER 0
RS_SNDCHR:						; SEND CHARACTER OUT
RS_SNDCHR_WAIT:
	LD	B, A
	XOR	A				; SELECT READ REGISTER 0
	OUT	(CMD_CH), A
	IN	A, (C)				; GET REGISTER 0 VALUE
	AND	RS_TRANSMIT_PENDING_MASK	; IS TRANSMIT PENDING?
	JR	Z, RS_SNDCHR_WAIT		; YES, THEN WAIT UNTIL TRANSMIT COMPLETED
	LD	A, B
	SIO_OUT_A				; LOAD BYTE TO TRANSMIT
	RET

CLOSE:		 		; close RS232C port
	XOR	A		; MARK AS CLOSED AND RTS OFF
	LD	(RS_FLAGS), A
	SIO_CHIP_RTS	CMD_CH, SIO_RTSOFF
	RET


EOF:			 	; tell EOF code received
	RET

LOC:
	LD	A, (RS_DATCNT)	; BUFFER UTILIZATION COUNT
	ld	L, A
	ld	H, 0
	RET			; receiver buffer

LOF:			 	; reports number of free space left in the
	RET			; receiver buffer

BACKUP:				; back up a character
	RET

SNDBRK:				; send break character
	RET

DTR:				; turn on/off DTR line
	RET

SETCHN:				; set channel number
	RET

