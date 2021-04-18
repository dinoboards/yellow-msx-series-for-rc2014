
RAMAD0	equ	0F341h	; Slot of the Main-Ram on page 0~3FFFh
EXTBIO	equ	0ffcah	; Call address to an extended Bios
CALSLT:      EQU    0001Ch
EXPTBL:      EQU    0FCC1h
RDSLT	EQU	0000CH

RS232_FN_COUNT:	EQU	13	; EXTENDED BIOS RS232 HAS 13 JUMP INSTRUCTIONS

	SECTION	CODE

;
; extern void    extbio_get_dev(uint8_t* table) __z88dk_fastcall;
;
	PUBLIC	_extbio_get_dev
_extbio_get_dev:
	CALL	GETSLT		; B = NUMBER THE SLOT OF THE TABLE
	LD	DE, 0		; BROAD-CAST, FUNCTION: 'GET DEVICE NUMBER'
	JP	EXTBIO

;
; extern void extbio_get_dev_info_table(uint8_t device_id, extbio_info* info_table);
;
	PUBLIC	_extbio_get_dev_info_table
_extbio_get_dev_info_table:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	CALL	GETSLT		; B = number the slot of the table

	LD	D, (IX+4) 	; DEVICE_ID
	LD	L, (IX+5)  	; INFO_TABLE
	LD	H, (IX+6)

	LD	E, 0		; FUNCTION 'GET INFO'
	CALL	EXTBIO

	POP	IX
	RET
;
; extern void rs232_link(extbio_info *p) __z88dk_fastcall;
;
	PUBLIC	_rs232_link
_rs232_link:
	LD	A, (HL)				; RETRIEVE SLOT ID
	ld	de, rs232_slot_init+1		; WRITE IT TO THE LINK FUNCTIONS
	ld	b,  RS232_FN_COUNT
loop1:
	ld	(de), a
	inc	de
	inc	de
	inc	de
	inc	de
	inc	de
	djnz	loop1

	INC	HL				; RETRIEVE THE JUMP TABLE ADDRESS
	LD	E, (HL)
	INC	HL
	LD	D, (HL)				; DE IS JUMP TABLE ADDR - FIRST 3 ARE IGNORED, FOLLOWED BY JUMPS
	INC	DE
	INC	DE
	INC	DE


	LD	HL, rs232_slot_init+2		; WRITE THE JUMP ADDRESSES
	LD	B, RS232_FN_COUNT
LOOP:
	ld	(hl), e				; WRITE JUMP TABLE REF TO LINK FUNCTION
	inc	hl
	ld	(hl), d
	inc	hl

	inc	hl
	inc	hl
	inc	hl				; HL REFERENCES NEXT LOCAL LINK FUNCTION

	INC	DE
	INC	DE
	INC	DE				; DE NOW PTS TO NEXT JUMP INSTRUCTION

	DJNZ	LOOP

	RET
;
; extern void rs232_init(rs232_init_params*) __z88dk_fastcall;
;
	PUBLIC	_rs232_init
_rs232_init:
	CALL	GETSLT			; B = number the slot of the table
	JP	rs232_slot_init

	PUBLIC	_rs232_open
_rs232_open:
	JP	rs232_slot_open

	PUBLIC	_rs232_close
_rs232_close:
	JP	rs232_slot_close

;extern uint8_t rs232_sndchr(const char ch) __z88dk_fastcall;
	PUBLIC	_rs232_sndchr
_rs232_sndchr:
	LD	A, L
	CALL	rs232_slot_sndchr
	LD	L, 0
	RET	Z
	LD	L, 1
	RET

;
; extern uint16_t rs232_getchr();
;
	PUBLIC	_rs232_getchr
_rs232_getchr:
	CALL	rs232_slot_getchr
	LD	H, 0
	LD	L, A
	RET	Z
	LD	H, 1
	RET

;----------------------------------------------------------------
; ROUTINE GETSLT
; ENTRY: HL = ADDRESS IN RAM
; OUTPUT: B = SLOT NUMBER CORRESPONDING TO THE HL ADDRESS
; MODIFIES: A AND B

GETSLT:
	PUSH	HL
	LD	A,H
	RLA
	RLA
	RLA		; BIT 6-7 TO BIT 1-0
	AND	3	; RESET UNUSED BITS
	LD	C,A
	LD	B,0
	LD	HL,RAMAD0
	ADD	HL,BC
	LD	B,(HL)	; B = SLOT NUMBER OF MAIN RAM
	POP	HL
	RET

	PUBLIC	_rs232_slot_init
_rs232_slot_init:
rs232_slot_jumps:
rs232_slot_init:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_open:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_stat:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_getchr:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_sndchr:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_close:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_eof:
	RST	30H
	DB	0
	DW	0
	RET

;
; extern uint16_t rs232_loc();
;
	PUBLIC	_rs232_loc
_rs232_loc:
rs232_slot_loc:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_lof:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_backup:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_sndbrk:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_dtr:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_setchn:
	RST	30H
	DB	0
	DW	0
	RET


HIMEM	EQU	0FC4AH		;TOP ADDRESS OF RAM WHICH CAN BE USED
BOTTOM	EQU	0FC48H		;POINTER TO BOTTOM OF RAM
STKTOP	EQU	0F674H		;POINTER TO BOTTOM OF STACK
MEMSIZ	EQU	0F672H		;POINTER TO END OF STRING SPACE
NULBUF	EQU	0F862H		;POINTER TO BUFFER #0
FILTAB	EQU	0F860H		;POINTER TO FILE POINTER TABLE
SAVSTK	EQU	0F6B1H		;POINTER TO VALID STACK BOTTOM

; call_bios:   ld     iy,(EXPTBL-1)       ;BIOS slot in iyh
;              jp   CALSLT              ;interslot call



;-----------------------------------------------------------------------------------------
; BELOW THIS MARK IS A DUPLICATION OF THE RS232 DRIVER - FOR DRIVER DEVELOPMENT PURPOSE ONLY

; MSX VARIABLES
MEXBIH		EQU	0FB07h  ; EXTENDED BIOS HOOK CALLED BY THE RS-232C
RS232WRK	EQU	MEXBIH	; REPORPOSE VARIABLE

; SIO SETTINGS
SIO0BASE	EQU	080h		; SIO BASE PORT
SIO0A_DAT	EQU	SIO0BASE + 1	; DATA PORT
SIO0A_CMD	EQU	SIO0BASE + 0	; CONTROL/STATUS PORT
SIO0B_CMD	EQU	SIO0BASE + 2	; DATA PORT
SIO0B_DAT	EQU	SIO0BASE + 3    ; CONTROL/STATUS PORT
DAT_CH		EQU	SIO0B_DAT	; DRIVER CURRENTLY ONLY SUPPORTS CHANNEL B OF THE SIO/2
CMD_CH		EQU	SIO0B_CMD
SIO_WR1VAL	EQU	018h		; WR1 VALUE FOR INT ON RECEIVED CHARS
SIO_RTSON	EQU	0EAh
SIO_RTSOFF	EQU	0E8h
SIO_RTS:	DB	0FFh		; 0 => RTS OFF, $FF => RTS ON


	PUBLIC	_DRV_INIT_SIO
_DRV_INIT_SIO:
DRV_INIT_SIO:
	LD	HL, (RS232WRK)		; RETRIEVE INT_HANDLER ADDRESS

	EX	DE, HL			; COPY INTERRUPT HANDLER INTO WORK ARA
	LD	HL, SIO_INT
	LD	BC, SIO_INT_LEN
	LDIR

	LD	DE, MSG_SIO
	CALL	PRINT

	CALL	SIO_PROBE
	JR	Z, SIO_FOUND

	LD	DE, MSG_NOT
	CALL	PRINT

SIO_FOUND:
	LD	DE, MSG_PRESENT
	JP	PRINT

MSG_SIO:
	DB	"SIO/2 Module:        ", 9, 0

MSG_PRESENT:
	DB	"PRESENT", 13, 10, 0

MSG_NOT:
	DB	"NOT ", 0

	PUBLIC	_RS232_JUMP_TABLE
_RS232_JUMP_TABLE:
RS232_JUMP_TABLE:
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


INIT:				; initialize RS232C port
	CALL	SIO_INIT
	RET

OPEN:		; open RS232C port
	CALL	RS232_ENA_INTERRUPTS
	RET
STAT:		; Read Status
	RET
GETCHR:		; reveive data

	RET

;---------------------------------------------------------------------------------
;        Entry:  [A] = character to send
;        Return: carry flag is set if control break key was pressed
;                zero flag is set if time out error occured during waiting for
;                XON or/and CTS signal.
;        Modify: [F]
;
;        Description:
;         Send  specified character to RS232C port. The character flow control
;         by XON/XOFF  characters and/or  CTS (Clear  To Send)  line signal is
;         handled  if initialized  so. Time  out error  will be generated when
;         specified time passed during waiting for permision for transmission,
;         and the character will not be sent.
;
; DOES NOT SUPPORT FLOW CONTROL
; CURRENT ONLY SUPPORT CHANNEL B
SNDCHR:
	OUT	(DAT_CH), A		; send to port
	XOR	A			; SIGNAL SUCCESS
	RET

CLOSE:		; close RS232C port
	CALL	RS232_DIS_INTERRUPTS
	RET
EOF:		; tell EOF code received
	RET

LOC:
	LD	HL, 16 		; reports number of characters in the
	RET			; receiver buffer

LOF:		; reports number of free space left in the
;: receiver buffer
	RET
BACKUP:		; back up a character
	RET
SNDBRK:		; send break character
	RET
DTR:		; turn on/off DTR line
	RET
SETCHN:		; set channel number
	RET

; --------------------------------------
; RS232 support functions

; RS232 SYSTEM VARIABLES
OLDINT		EQU	0FB0Ch	; INTERRUPT HOOK CALLED BY THE RS-232C (5 BYTES)
RSFCB		EQU	0FB04h	; FCB ("FILE CONTROL BLOCK") ADDRESS OF THE RS-232C
RSIQLN		EQU	0FB06h	; MAX NUMBER OF CHARACTERS THAT CAN BE STORED AT BUFFER RSFCB
; called at start of interrupt subroutine, before it has been checked if
; the interrupt was from VDP; used by e.g. RS232
H_KEYI		EQU	0FD9Ah

; ----------------------------------------
; HOOK IN INTERRUPT HANDLER
; MODIFY HANDLER FOR CURRENT BUFFER CONFIGURATION

RS232_ENA_INTERRUPTS:
	EX	DE, HL
	LD	HL, (RS232WRK)
	ADD	HL, SIO_RCVBUF
	LD	(HL), E
	INC	HL
	LD	(HL), D
	DI

	LD	DE, OLDINT		; GET ADDRESS OF OLD INT. HOOK SAVED AREA
	LD	HL, H_KEYI		; GET ADDRESS OF INTERRUPT ENTRY HOOK
	LD	BC, 5			; LENGHT OF HOOK IS 5 BYTES
	LDIR				; TRANSFER

if 0 ; ROM DRIVER VERSION
	XOR	A
	LD	IX, GSLOT1
	CALL	CALBNK
else
	LD	A, 10000111B		; slot 3 1
endif
	LD	(H_KEYI+1), A		; SET SLOT ADDRESS
	LD	A, $F7			; 'RST 30H' INTER-SLOT CALL OPERATION CODE
	LD	(H_KEYI), A		; SET NEW HOOK OP-CODE
	LD	HL, SIO_INT		; GET OUR INTERRUPT ENTRY POINT
	LD	(H_KEYI+2), HL		; SET NEW INTERRUPT ENTRY POINT
	LD	A, $C9			; 'RET' OPERATION CODE
	LD	(H_KEYI+4), A		; SET OPERATION CODE OF 'RET'
	EI

	RET

RS232_DIS_INTERRUPTS:
	DI
	LD	A, 1			; TURN OFF INTERRUIPTS FOR CHANNEL 0
	OUT	(SIO0A_CMD), A
	LD	A, 0
	OUT	(SIO0A_CMD),A

	LD	A, 1			; TURN OFF INTERRUPTS FOR CHANNEL 1
	OUT	(SIO0B_CMD), A
	LD	A, 0
	OUT	(SIO0B_CMD),A

	LD	DE, H_KEYI		; REMOVE INTERRUPT HOOK
	LD	HL, OLDINT
	LD	BC, 5
	LDIR
	EI

	RET


;---------------------------------------------------
; SIO FUNCTIONS


; -----------------------------------
;
; INITIALISE BOTH CHANNELS OF SIO/2 CHIP
;
SIO_INIT:
	LD	BC, SIO_RTSOFF*256 + SIO0A_CMD
	CALL	SIOINIT
	LD	BC, SIO_RTSON*256 + SIO0B_CMD
	JP	SIOINIT

; -----------------------------------------------------------------------------
;
; INITIALISE SIO CHANNEL
; INPUTS:
;	B - RTS PORT STATE
;	C - SIO CHANNEL PORT
SIOINIT:
	LD	A, 0
	OUT	(C), A
	LD	A, 018H
	OUT	(C), A

	; WR4: CLK BAUD PARITY STOP BIT
	LD	A, 4
	OUT	(C), A
	LD	A, 0C4H
	OUT	(C), A

	; WR1: INTERRUPT OFF
	XOR	A

	; ; WR1: INTERRUPT ON
	; LD	A, 1
	OUT	(C), A
	LD	A, SIO_WR1VAL
	OUT	(C), A

	; WR2: IM2 VEC OFFSET
	LD	A, 2
	OUT	(C), A
	LD	A, 00H
	OUT	(C),A

	; WR3: 8 BIT RCV, CTS/DCD AUTO, RX ENABLE
	LD	A, 3
	OUT	(C), A
	LD	A, 0E1H
	OUT	(C), A

	; WR5: DTR, 8
	LD	A, 5
	OUT	(C), A
	OUT	(C), B
	RET


; SIO CHIP PROBE
; CHECK FOR PRESENCE OF SIO CHIPS AND POPULATE THE
; SIO_MAP BITMAP (ONE BIT PER CHIP).  THIS DETECTS
; CHIPS, NOT CHANNELS.  EACH CHIP HAS 2 CHANNELS.
; MAX OF TWO CHIPS CURRENTLY.  INT VEC VALUE IS TRASHED!
;
	PUBLIC	SIO_PROBE
SIO_PROBE:
	; INIT THE INT VEC REGISTER OF ALL POSSIBLE CHIPS
	; TO ZERO.
	XOR	A
	LD	B, 2			; WR2 REGISTER (INT VEC)
	LD	C, SIO0B_CMD		; FIRST CHIP
	CALL	SIO_WR			; WRITE ZERO TO CHIP REG

	; FIRST POSSIBLE CHIP
	LD	C, SIO0B_CMD		; FIRST CHIP CMD/STAT PORT
	CALL	SIO_PROBECHIP		; PROBE IT
	RET
;
SIO_PROBECHIP:
	; READ WR2 TO ENSURE IT IS ZERO (AVOID PHANTOM PORTS)
	CALL	SIO_RD			; GET VALUE
	AND	0F0h			; ONLY TOP NIBBLE
	RET	NZ			; ABORT IF NOT ZERO
	; WRITE INT VEC VALUE TO WR2
	LD	A, 0FFh			; TEST VALUE
	CALL	SIO_WR			; WRITE IT
	; READ WR2 TO CONFIRM VALUE WRITTEN
	CALL	SIO_RD			; REREAD VALUE
	AND	0F0h			; ONLY TOP NIBBLE
	CP	0F0h			; COMPARE
	RET

SIO_WR:
	OUT	(C), B			; SELECT CHIP REGISTER
	OUT	(C), A			; WRITE VALUE
	RET
;
SIO_RD:
	OUT	(C), B			; SELECT CHIP REGISTER
	IN	A, (C)			; GET VALUE
	RET

; ---------------------------------------
; COPIED TO WORK AREA IN PAGE 3
;
SIO_INT:
	DI				; INTERRUPTS WILL BE RE-ENABLED BY MSX BIOS

	; CHECK TO SEE IF SOMETHING IS ACTUALLY THERE
	XOR	A			; READ REGISTER 0
	OUT	(CMD_CH), A
	IN	A, (CMD_CH)
	AND	01			; ISOLATE RECEIVE READY BIT
	RET	Z			; NOTHING AVAILABLE ON CURRENT CHANNEL

SIO_INTRCV1:
	; RECEIVE CHARACTER INTO BUFFER
	IN	A, (DAT_CH)			; READ PORT

	LD	B, A			; SAVE BYTE READ
	LD	HL, 0			; SIO_RCVBUF
SIO_RCVBUF	equ	($-2)-SIO_INT

	LD	A, (HL)			; GET COUNT
	CP	0			; COMPARE TO BUFFER SIZE
SIO_BUFSZ	EQU	($-1)-SIO_INT

	JR	Z, SIO_INTRCV4		; BAIL OUT IF BUFFER FULL, RCV BYTE DISCARDED
	INC	A			; INCREMENT THE COUNT
	LD	(HL), A			; AND SAVE IT

	CP	0			; BUFFER GETTING FULL?
SIO_BUF_HI	EQU	($-1)-SIO_INT

	JR	NZ, SIO_INTRCV2		; IF NOT, BYPASS CLEARING RTS

	LD	A, 5			; SELECTED REGISTER WRITE 5
	OUT	(CMD_CH), A
	LD	A, SIO_RTSOFF
	OUT	(CMD_CH), A

	XOR	A
	LD	(SIO_RTS), A

SIO_INTRCV2:
	INC	HL			; HL NOW HAS ADR OF HEAD PTR
	PUSH	HL			; SAVE ADR OF HEAD PTR
	LD	A, (HL)			; DEREFERENCE HL
	INC	HL
	LD	H, (HL)
	LD	L, A			; HL IS NOW ACTUAL HEAD PTR
	LD	(HL), B			; SAVE CHARACTER RECEIVED IN BUFFER AT HEAD

	INC	HL			; BUMP HEAD POINTER
	POP	DE			; RECOVER ADR OF HEAD PTR
	LD	A, L			; GET LOW BYTE OF HEAD PTR
	SUB	SIO_BUFSZ+4			; SUBTRACT SIZE OF BUFFER AND POINTER
	CP	E			; IF EQUAL TO START, HEAD PTR IS PAST BUF END
	JR	NZ, SIO_INTRCV3		; IF NOT, BYPASS
	LD	H, D			; SET HL TO
	LD	L, E			; ... HEAD PTR ADR
	INC	HL			; BUMP PAST HEAD PTR
	INC	HL
	INC	HL
	INC	HL			; ... SO HL NOW HAS ADR OF ACTUAL BUFFER START
SIO_INTRCV3:
	EX	DE, HL			; DE := HEAD PTR VAL, HL := ADR OF HEAD PTR
	LD	(HL), E			; SAVE UPDATED HEAD PTR
	INC	HL
	LD	(HL), D
	; CHECK FOR MORE PENDING...

	XOR	A
	OUT	(CMD_CH), A		; READ REGISTER 0
	IN	A, (CMD_CH)		;
	RRA				; READY BIT TO CF
	JR	C, SIO_INTRCV1		; IF SET, DO SOME MORE

	LD	A, (SIO_RTS)
	OR	A
	JR	Z, SIO_INTRCV4		; ABORT NOW IF RTS IS OFF

	; TEST FOR NEW BYTES FOR A SHORT PERIOD OF TIME
	LD	B, 40
SIO_MORE:
	IN	A, (CMD_CH)		;
	RRA				; READY BIT TO CF
	JR	C, SIO_INTRCV1		; IF SET, DO SOME MORE
	DJNZ	SIO_MORE

SIO_INTRCV4:
	; RESET INTERRUPT STATE IN SIO (CHANNEL A CONTROLS CHANNEL B ALSO)
	LD	A, 0
	OUT	(SIO0A_CMD), A
	LD	A, 038h
	OUT	(SIO0A_CMD), A
	RET

SIO_INT_LEN	EQU	$-SIO_INT

	PUBLIC	SIOLEN
SIOLEN	EQU	SIO_INT_LEN

;-----------------------------------------------------------------------------
;
; PRINT A ZERO-TERMINATED STRING ON SCREEN
; INPUT: DE = STRING ADDRESS
BDOS	EQU	$0005
PRINT:
	PUSH	HL
	EX	DE, HL
PRINTLP:
	LD	A, (HL)
	OR	A
	LD	E, A
	JR	Z, PRINTEND
	LD	C, 2
	push	hl
	CALL	BDOS
	pop	hl
	INC	HL
	JR	PRINTLP
PRINTEND:
	POP	HL
	RET
