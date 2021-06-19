
FOSSIL_JUMP_TABLE_REF	EQU	0F3FEh
FOSSIL_JUMP_TABLE_SLOT	EQU	0F400h
FOSSIL_MARK_1		EQU	0xf3fc
FOSSIL_MARK_2		EQU	0xf3fd

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

INIT:				; initialize RS232C port
	DI
	SIO_INIT	SIO0A_CMD, SIO_RTSOFF
	SIO_INIT	SIO0B_CMD, SIO_RTSON
	EI
	XOR	A
	RET

; ------------------------------------------------
OPEN:				; open RS232C port
	XOR	A
	LD	(DATCNT), A

	LD	(RSFCB), HL
        LD      A, C
        LD      (RSIQLN),A

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

	LD      HL, FLAGS		; ENSURE RS232 IS MARKED AS OPENED
	SET     3, (HL)
	RET

STAT:				; Read Status
	RET

GETCHR:				; reveive data
	CALL	SIO_RCBBYT
	OR	A		; CLEAR CARRY FLAG (NEVER HAVE EOF)
	RET

SNDCHR:				; send data
	push	af
	SIO_OUT_A
	pop	af
	BIOS_FN	CHPUT
	RET

CLOSE:		 		; close RS232C port
	RET


EOF:			 	; tell EOF code received
	RET

LOC:
	LD	A, (DATCNT)	; BUFFER UTILIZATION COUNT
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

FOSSIL_JUMP_TABLE:
; _fossil_getversion:	JP	0	; 0
; _fossil_init:		JP	0	; 1
; _fossil_deinit:		JP	0	; 2
; __fossil_set_baud:	JP	0	; 3
; fossil_set_protocol:	JP	0	; 4
; fossil_channel:		JP	0	; 5
; fossil_rs_in:		JP	0	; 6
; fossil_rs_out:		JP	0	; 7
; fossil_rs_in_stat:	JP	0	; 8
; _fossil_rs_out_stat:	JP	0	; 9
; fossil_dtr:		JP	0	; 10
; fossil_rts:		JP	0	; 11
; fossil_carrier:		JP	0	; 12
; _fossil_chars_in_buf:	JP	0	; 13
; _fossil_size_of_buf:	JP	0	; 14
; _fossil_flush_buf:	JP	0	; 15
; fossil_fastint:		JP	0	; 16
; fossil_hookstat:	JP	0	; 17
; _fossil_chput_hook:	JP	0	; 18
; _fossil_keyb_hook:	JP	0	; 19

