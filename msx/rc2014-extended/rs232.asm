
FOSSIL_JUMP_TABLE_REF	EQU	0F3FEh
FOSSIL_JUMP_TABLE_SLOT	EQU	0F400h
FOSSIL_MARK_1		EQU	0xf3fc
FOSSIL_MARK_2		EQU	0xf3fd


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
	LD	DE, MSG
	CALL	PRINT

	XOR	A
	RET


MSG:	DB	"Inited.", 13, 10, 0

; ------------------------------------------------
OPEN:				; open RS232C port
	LD	DE, MSG2
	CALL	PRINT
	RET

MSG2:	DB	"Opened", 13, 10, 0

STAT:				; Read Status
	RET

GETCHR:				; reveive data
	XOR	A
	LD	A, 'x'
	RET

SNDCHR:				; send data
	push	af
	SIO_OUT_A
	pop	af
	BIOS_FN	CHPUT
	RET

CLOSE:		 		; close RS232C port
	; LD	DE, MSG3
	; CALL	PRINT

	;CALL	REMOVE_INTERRUPT_HANDLER
	RET

; MSG3:	DB	"Are we closed yet.\r\n", 0


EOF:			 	; tell EOF code received
	RET

LOC:
	LD	HL, 16 		; reports number of characters in the
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

