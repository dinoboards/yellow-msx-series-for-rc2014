
	include	"sio.inc"
	include	"msx.inc"

;
; All the code here is relocated to page 3 location during rominit

SIO_BUFSZ	EQU	150

FOSSIL_JUMP_TABLE:
	jp	getversion	; Version H.L (H,L packed BCD)
	jp	init		; initialises RS232
	jp	deinit		; deinitialises the RS232

	jp	set_baud	; H=Tx baud rate, L=Rx baud rate
				; 0  = 75	 1  = 300	  2  = 600
				; 3  = 1200	 4  = 2400	  5  = 4800
				; 6  = 9600	 7  = 19200	  8  = 38400
				; 9  = 57600	 11 = 115200

; if < 5, then 5
; if > 7, then 7


; Extra information :
;	In register H and L, the driver reports the actually selected rate.
;	This is done in case the hardware does not support the selected
;	rate. If the driver does not support the selected rate, it selects
;	the highest possible rate below the selected.

	jp	set_protocol	; H 0-1 data bits
				;	00 5 bits or less
				;	01 6 bits
				;	10 7 bits
				;	11 8 bits
				;   2-3 stop bits
				;	01 1 stopbit
				;	10 1.5 stopbits
				;	11 2 stopbits
				;   4-5 parity
				;	00 none
				;	01 even
				;	11 odd
				;   6-7 0
				; L = 0 (has to be set to zero, for future
				;	 extensions!)
	jp	channel 	; H	channel number (0-1) (NMS only)

	jp	rs_in		; read from RS232 buffer, A = character/byte
	jp	rs_out		; write to RS232, A = character/byte

	jp	rs_in_stat	; A=0 No data in buffer, A!=0 data in buffer
				; The F register is set according the result
	jp	rs_out_stat	; At this time every time=!0, but this is
				; reserved for future expansion of the driver
				; to sent data on interrupt. This function
				; is not needed to use, rs_in_stat is required
				; before rs_in is done.

	jp	dtr		; H=0 drop DTR, H=255 raise DTR
	jp	rts		; H=0 drop RTS, H=255 raise RTS

	jp	carrier 	; A=0 no carrier, A!= carrier detect

	jp	chars_in_buf	; Return : HL = characters in Rx buf.
	jp	size_of_buf	; Return : HL = size of buffer
	jp	flushbuf	; FLUSH THE RECEIVE BUFFER

	jp	fastint		; use &H0038 hook for speedup
				; H=0 Connect driver fast
				; H=1 Release fast hook

; This fastint needs some extra attention:
; The driver uses hook &HFD9A, but this is not very fast. Since this hook
; is called after some BIOS work, and after this hook, the BIOS continues
; to do some time waisting things.
; Therefore, this driver has the option to install itselve at the &H38 hook.
; This can be done if there is RAM available at &H0000-&H3FFF. This is the
; case in DOS(2).

	jp	hook38stat	; set status for 0038 hook
				; h  = 0, every interrupt is supported
				; h != 0, only RS232 interrupt (and VDP)

; This function is for use with fastint.
; When the "connect driver fast" has been issued, you can control how the
; interrupt handler of the driver behaves.
; If every interrupt is supported, there is slightly more processor time
; used after an interrupt from the RS232C interface.

	jp	chput_hook	; redirect CHPUT data to RS232
				; H=0 no redirection
				; H=1 redirect with echo
				; H=3 redirect without echo (faster)

; All print commands that would be issued using the BIOS calls, will be
; transmitted through the RS232C interface.

	jp	keyb_hook	; redirect RS232 to keyboard buffer
				; H=0 release hook, H!=0 bend hook

; With this function, you can redirect all incoming RS232C data to the
; keyboard buffer.
; This and the previous function are nice to let a terminal control your
; MSX computer.

	jp	get_info	; Gives a pointer in HL that points to a
				; driver info block. This block describes
				; the current situation about the driver.

getversion:
	LD	HL, 0x0101
	RET

init:
	PUSH	IX
	PUSH	IY
	LD	HL, RS232_INIT_TABLE
	RST	30H
	DB	$8F		; SLOT 3-3
	DW	RSF_INIT
	EI

	LD	HL, SIO_RCVBUF
	LD	C, SIO_BUFSZ
	LD	E, 4

	RST	30H
	DB	$8F		; SLOT 3-3
	DW	RS_OPEN
	EI
	POP	IY
	POP	IX
	RET

deinit:
	XOR	A		; MARK AS CLOSED AND RTS OFF
	LD	(RS_FLAGS), A
	SIO_CHIP_RTS	CMD_CH, SIO_RTSOFF
	RET

set_baud:
 	LD	A, L
	CP	H
	JR	NC, SKIP1
	LD	A, H		; H IS LARGER, SO USE THAT AS BASIS

SKIP1:
	CP	5
	JR	NC, SKIP2	; LESS THAN 5
	LD	A, 5		; SET TO 5
SKIP2:
	CP 	7
	JR	C, SKIP3	; GREATER THAN 7
	LD	A, 7		; SET TO 7

SKIP3:
	LD	HL, RSC_RCV_BAUD
	CP	7
	JR	NZ, SKIP4
	LD	HL, BAUD_HIGH
	JR	SET_BAUD_EXIT

SKIP4:
	CP	6
	JR	NZ, SKIP5
	LD	HL, BAUD_MID
	JR	SET_BAUD_EXIT

SKIP5:
	LD	HL, BAUD_LOW

SET_BAUD_EXIT:
	LD	(RSC_RCV_BAUD), HL
	LD	(RSC_SND_BAUD), HL

	LD	L, A		; RETURN THE ACTUAL SELECTED BAUD RATES
	LD	H, A
	RET

rs_in:
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
	LD	A, C			; MOVE CHAR TO RETURN TO A
	OR	A
	RET

rs_out:
	PUSH	IX
	PUSH	IY
	RST	$30
	DB	$8F
	DW	RS_SNDCHR
	EI
	POP	IY
	POP	IX
	RET

rs_in_stat:
	LD	A, (RS_DATCNT)
	OR	A
	RET


chars_in_buf:
	LD	A, (RS_DATCNT)
	LD	L, A
	LD	H, 0

set_protocol:
channel:
rs_out_stat:
dtr:
carrier:
rts:
size_of_buf:
flushbuf:
fastint:
hook38stat:
chput_hook:
keyb_hook:
get_info:
	RET

	PUBLIC	SIO_INT
SIO_INT:
	DI				; INTERRUPTS WILL BE RE-ENABLED BY MSX BIOS

	; CHECK TO SEE IF SOMETHING IS ACTUALLY THERE
	XOR	A			; READ REGISTER 0
	OUT	(CMD_CH), A
	IN	A, (CMD_CH)
	AND	$01			; ISOLATE RECEIVE READY BIT
	JP	Z, RS_OLDINT		; NOTHING AVAILABLE ON CURRENT CHANNEL

	LD	A, (RS_IQLN)
	EXX
	LD	C, A			; BUFFER FULL COUNT IN C
	LD	HL, RS_DATCNT
	SUB	5			; 5 FROM BUF SIZE IS HIGH WATER MARK
	JR 	Z, SIO_ZERO_HI_MARK
	JR 	NC, SIO_5_MINUS_HI_MARK
SIO_ZERO_HI_MARK:
	LD	A, 1
SIO_5_MINUS_HI_MARK:
	LD	D, A
	EXX

	LD	HL, RS_FLAGS		; IS OPENED?
	BIT	3, (HL)			; FLAG PORT OPEN?
	JR	Z, SIO_INT_ABORT

	LD	A, (RS_BUFEND)		; GET BUFEND PTR LOW BYTE
	LD	C, A

	LD	HL, (RS_FCB)
	LD	D, H			; SAVE ADR OF HEAD PTR
	LD	E, L
	LD	A, (HL)			; DEREFERENCE HL
	INC	HL
	LD	H, (HL)
	LD	L, A			; HL IS NOW ACTUAL HEAD PTR

	; HL IS HEAD - ADDR OF NEXT BYTE TO BE WRITTEN IN BUFFER
	; DE IS &HEAD - ADDR STORE FOR HEAD PTR

	; RECEIVE CHARACTER INTO BUFFER
SIO_INTRCV1:
	IN	A, (DAT_CH)		; READ PORT
	LD	B, A			; SAVE BYTE READ

	EXX
	LD	A, (HL)			; GET COUNT
	CP	C			; COMPARE TO BUFFER SIZE
	JR	Z, SIO_INTRCV4		; BAIL OUT IF BUFFER FULL, RCV BYTE DISCARDED

	INC	A			; INCREMENT THE COUNT
	LD	(HL), A			; AND SAVE IT
	CP	D			; HIT HIGH WATER MARK?
	EXX
	JR	NZ, SIO_INTRCV2		; IF NOT, BYPASS CLEARING RTS

	SIO_CHIP_RTS	CMD_CH, SIO_RTSOFF
	LD	A, (RS_FLAGS)		; SET BIT FLAG FOR RTS OFF
	RES	1, A
	LD	(RS_FLAGS), A

SIO_INTRCV2:
	LD	(HL), B			; SAVE CHARACTER RECEIVED IN BUFFER AT HEAD
	INC	HL			; BUMP HEAD POINTER

	LD	A, C			; GET BUFEND PTR LOW BYTE
	CP	L			; ARE WE AT BUFF END?
	JR	NZ, SIO_INTRCV3		; IF NOT, BYPASS
	LD	H, D			; SET HL TO
	LD	L, E			; ... HEAD PTR ADR
	INC	HL			; BUMP PAST HEAD PTR
	INC	HL
	INC	HL
	INC	HL			; ... SO HL NOW HAS ADR OF ACTUAL BUFFER START

SIO_INTRCV3:
	; CHECK FOR MORE PENDING...
	XOR	A
	OUT	(CMD_CH), A		; READ REGISTER 0
	IN	A, (CMD_CH)		;
	RRA				; READY BIT TO CF
	JR	C, SIO_INTRCV1		; IF SET, DO SOME MORE

	LD	A, (RS_FLAGS)
	BIT	1, A
	JR	Z, SIO_UPDATE_HEAD_PTR		; ABORT NOW IF RTS IS OFF

	; TEST FOR NEW BYTES FOR A SHORT PERIOD OF TIME
	LD	B, 80
SIO_MORE:
	IN	A, (CMD_CH)		;
	RRA				; READY BIT TO CF
	JR	C, SIO_INTRCV1		; IF SET, DO SOME MORE
	DJNZ	SIO_MORE

COMMAND_0	EQU	0
COMMAND_1	EQU	0x08
COMMAND_2	EQU	0x10
COMMAND_3	EQU	0x18
COMMAND_4	EQU	0x20
COMMAND_5	EQU	0x28
COMMAND_6	EQU	0x30
COMMAND_7	EQU	0x38

SIO_UPDATE_HEAD_PTR:
	EX	DE, HL			; DE := HEAD PTR VAL, HL := ADR OF HEAD PTR
	LD	(HL), E			; SAVE UPDATED HEAD PTR
	INC	HL
	LD	(HL), D


SIO_INTRCV4:
	; NOT SURE WHY NEED TO RESET CHANNEL A
	; SOMETHING NOT QUITE RIGHT
	LD	A, 0
	OUT	(SIO0A_CMD), A
	LD	A, COMMAND_3
	OUT	(SIO0A_CMD), A
	JP	RS_OLDINT

SIO_INT_ABORT:
	IN	A, (DAT_CH)
	; PORT NOT OPENED, SO IGNORE BYTE, RESET RTS AND EXIT
	SIO_CHIP_RTS	CMD_CH, SIO_RTSOFF
	RES	1, (HL)			; SET BIT FLAG FOR RTS OFF
	JR	SIO_INTRCV4

RS232_INIT_TABLE:
RSC_CHAR_LEN:		DB	'8'		; Character length '5'-'8'
RSC_PARITY:		DB	'N'		; Parity 'E','O','I','N'
RSC_STOP_BITS:		DB	'1'		; Stop bits '1','2','3'
RSC_XON_XOFF:		DB	'N'		; XON/XOFF controll 'X','N'
RSC_CTR_RTS:		DB	'H'		; CTR-RTS hand shake 'H','N'
RSC_AUTO_RCV_LF:	DB	'N'		; Auto LF for receive 'A','N'
RSC_AUTO_SND_LF:	DB	'N'		; Auto LF for send 'A','N'
RSC_SI_SO_CTRL:		DB	'N'		; SI/SO control 'S','N'
RSC_RCV_BAUD:		DW	19200		; Receiver baud rate  50-19200 ; MARK MAKE BAUD RATE CONFIG
RSC_SND_BAUD:		DW	19200		; Transmitter baud rate 50-19200 ; MARK MAKE BAUD RATE CONFIG
RSC_TIMEOUT_CNT:	DB	0		; Time out counter 0-255

SIO_RCVBUF:
SIO_HD:		DW	SIO_BUF		; BUFFER HEAD POINTER
SIO_TL:		DW	SIO_BUF		; BUFFER TAIL POINTER
SIO_BUF:	DS	SIO_BUFSZ, $00	; RECEIVE RING BUFFER
