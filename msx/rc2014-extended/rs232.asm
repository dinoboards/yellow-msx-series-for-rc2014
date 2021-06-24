
; TODO
; [ ] INIT FUNCTION SHOULD ATTEMPT TO SET STOP BITS/PARITY/ETC, SPEED
; [ ] OPEN - ERROR UNLESS RAW MODE, ERROR IF ALREADY OPENED, ENABLE RTS
; [ ] STAT - IMPLEMENT
; [ ] SNDCHR XON/XOFF FLOW CONTROL
; [ ] CLOSE - DISABLE RTS
; [ ] EOF - ALWAYS FALSE
; [ ] LOF - IMPLEMENT
; [ ] SNDBRK - RESEARCH AND IMPLEMENT
; [ ] DTR - IMPLEMENT
; [ ] SETCHN - IMPLEMENT (SIO PORTS)

	include	"sio.inc"

EXTBIO_RS2_JUMP_TABLE:
	DEFB	0		; MSX serial features (no TxReady INT, No Sync detect, No Timer INT, No CD, No RI)
	DEFB	1		; version number
	DEFB	0		; reserved for future expansion
	JP	INIT		; initialize RS232C port
	JP	OPEN		; open RS232C port
	JP	STAT		; ReaD STATus
	JP	GETCHR		; reveive data
	JP	SNDCHR		; send data
	JP	CLOSE		; close RS232C port
	JP	EOF		; tell EOF code received
	JP	LOC		; reports number of characters in the
				; receiver buffer
	JP	LOF		; reports number of free space left in the
				; receiver buffer
	JP	BACKUP		; back up a character
	JP	SNDBRK		; send break character
	JP	DTR		; turn on/off DTR line
	JP	SETCHN		; set channel number


;------------------------------------
        ; Entry:  [HL]= address of the parameter table
        ;         [B] = slot address of the parameter table
        ; Return: carry flag is set if illegal parameters are contained
        ; Modify: [AF]

	PUBLIC	SIO_INIT
INIT:				; initialize RS232C port
SIO_INIT:
	DI

	XOR	A
	LD	(SIO_RTS), A

	SIO_CHIP_INIT	SIO0A_CMD, SIO_RTSOFF
	SIO_CHIP_INIT	SIO0B_CMD, SIO_RTSOFF
	EI
	XOR	A
	RET

; ------------------------------------------------
OPEN:				; open RS232C port
	EI
	XOR	A
	LD	(RS_DATCNT), A

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

	PUBLIC	SIO_OPEN
SIO_OPEN:
	LD      HL, RS_FLAGS		; ENSURE RS232 IS MARKED AS OPENED
	SET     3, (HL)

	LD	A, 254
	LD	(SIO_RTS), A
	SIO_CHIP_RTS        SIO0B_CMD, SIO_RTSON
	RET

STAT:				; Read Status
	RET

GETCHR:				; reveive data
	CALL	SIO_RCBBYT
	OR	A		; CLEAR CARRY FLAG (NEVER HAVE EOF)
	RET

SNDCHR:				; send data
	SIO_OUT_A
	RET

CLOSE:		 		; close RS232C port
	LD      HL, RS_FLAGS	; ENSURE RS232 IS MARKED AS CLOSED
	RES     3, (HL)

	XOR	A
	LD	(SIO_RTS), A
	SIO_CHIP_RTS        SIO0B_CMD, SIO_RTSOFF
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

