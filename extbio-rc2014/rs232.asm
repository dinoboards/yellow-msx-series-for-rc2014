
EXTBIO_JUMP_TABLE:
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
	LD	DE, MSG
	CALL	PRINT
	RET

MSG:	DB	"Are we inited yet.\r\n", 0

; ------------------------------------------------
OPEN:				; open RS232C port
	LD	DE, MSG2
	CALL	PRINT
	RET

MSG2:	DB	"Are we opened yet.\r\n", 0

	RET

STAT:				; Read Status
	RET

GETCHR:				; reveive data
	RET

SNDCHR:				; send data
	RET

CLOSE:		 		; close RS232C port
	RET

EOF:			 	; tell EOF code received
	RET

LOC:			 	; reports number of characters in the
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
