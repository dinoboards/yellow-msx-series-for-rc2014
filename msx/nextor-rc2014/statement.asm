
; Implement the following basic statements

; _VDP_SET_REG(REG_NUM, VALUE)
; set the VDP's register REG_NUM to VALUE.  Both REG_NUM and VALUE must be bytes
;
; _VDP_GET_REG(REG_NUM, <INT_VAR>)
; attempt to retrieve the specified register.  Only support with by the FPGA V9958 emulated VDP
; <INT_VAR> must be an integer variable to received the current value
;
; _VDP_GET_STATUS(REG_NUM, <INT_VAR>)
; retrieve the VDP's current status register value
; <INT_VAR> must be an integer variable to received the current value
;
; _SUPER_COLOR(R, G, B)
; for use with super modes. set the current VDP's `Colour Register` to the specified RGB value.
; R, G, B must be bytes
;

PROCNM		EQU	$FD89
FRMEVL		EQU	#4C64
VALTYP		EQU	$F663
DAC		EQU	$F7F6
CALBAS		EQU	$0159
GETBYT		EQU	$521C
FRMQNT		EQU	$542F
BASIC_ERR	EQU	$406F
CHRGTR		EQU	$4666
VDP_ADDR	EQU	$99
VDP_REGS:       equ     $9B             ; VDP register access (write only)

DRV_BASSTAT:
	PUSH	HL			; Save HL

	; HL points to PROCNM
	LD	HL, PROCNM

	; DE points to MY_STATEMENTS
	LD	DE, MY_STATEMENTS

; Compare strings
compare_loop:
	LD	A, (DE)
	OR	A
	JP	Z, no_statements  	; End of table, return

	PUSH	HL
	PUSH	DE

	; Point DE to the string in the table
	LD	A, (DE)
	LD	B, A
	INC	DE
	LD	A, (DE)
	LD	D, A
	LD	E, B

; Compare strings
compare_strings:
	LD	A, (DE)
	CP	(HL)
	JR	NZ, no_match
	INC	DE
	INC	HL
	LD	A, (DE)
	OR	A
	JR	NZ, compare_strings

; If we get here, the strings match
	POP	DE
	POP	HL

	INC	DE
	INC	DE
	EX	DE, HL
	JP	(HL)  			; Jump to the function

no_match:
	; If we get here, the strings don't match
	POP	DE
	POP	HL
	INC	DE
	INC	DE
	INC	DE
	INC	DE
	INC	DE
	JR	compare_loop


SUPER_COLOR_FN:
	POP	HL

	CALL	CHKCHAR
	DEFB	"("

	LD	IX,GETBYT
	CALL	CALBAS0

	PUSH	AF			; save red

	CALL	CHKCHAR
	DEFB	","

	LD	IX,GETBYT
	CALL	CALBAS0

	PUSH	AF			; save greem

	CALL	CHKCHAR
	DEFB	","

	LD	IX,GETBYT
	CALL	CALBAS0

	PUSH	AF			; save blue

	CALL	CHKCHAR
	DEFB	")"

	EXX				; protect HL

	POP	DE			; D = blue
	POP	BC			; B = green
	POP	HL			; H = red

	DI				; set bit 7 of R#30 to start RGB colour register loading
	LD	A, $80 | 1		; todo - do not assume colour res (bit 0) is set
	OUT	($99), A
	LD	A, $80 | 30
	OUT	($99), A

	LD	A, H
	OUT	($99), A		; load in Red
	LD	A, $80 | 29
	OUT	($99), A		; load into R#29

	LD	A, B
	OUT	($99), A		; load in Green
	LD	A, $80 | 29
	OUT	($99), A		; load into R#29

	LD	A, D
	OUT	($99), A		; load in Blue
	LD	A, $80 | 29
	OUT	($99), A		; load into R#29

	EI

	EXX				; restore HL
	AND	A		  	; Clear carry flag
	RET


VDP_SET_REG_FN:
	POP	 HL

	CALL	CHKCHAR
	DEFB	"("

	LD	IX,GETBYT
	CALL	CALBAS0

	PUSH	AF			; register number

	CALL	CHKCHAR
	DEFB	","

	LD	IX,GETBYT
	CALL	CALBAS0

	PUSH	AF			; register_value
	CALL	CHKCHAR
	DEFB	")"

	POP	AF
	POP	DE			; /D = register_number; A = register_value

	DI
	OUT	($99), A
	LD	A, D
	OR	$80
	OUT	($99), A
	EI

	AND	 A		   	; Clear carry flag
	RET

VDP_GET_REG_FN:
	POP	 HL

	CALL	CHKCHAR
	DEFB	"("

	LD	IX,GETBYT
	CALL	CALBAS0

	PUSH	AF			; register number

	CALL	CHKCHAR
	DEFB	","

	; capture variable to receive
	; value must be an int type
	LD	A, 0
	LD	($F6A5), A
	LD	IX, $5EA4
	CALL	CALBAS0

	// DE points to actual value
	// DE -2 is 2 byte NAME
	// DE - 3 is type

	PUSH		DE

	CALL	CHKCHAR
	DEFB	")"

	EXX

	POP	HL
	DEC	HL
	DEC	HL
	DEC	HL
	LD	A, (HL)
	CP	2			; Check if it's a int
	JR	NZ, TYPE_MISMATCH
	INC	HL
	INC	HL
	INC	HL			; HL points to variable storage

	POP	AF			; set reg 17 to A

	DI
	OUT	($99), A
	LD	A, 0x80 | 17
	OUT	($99), A
	IN	A, ($9B)		; request register value
	EI

	LD	(HL), a

	EXX

	AND	 A		  	 ; Clear carry flag
	RET

VDP_GET_STATUS_FN:
	POP	 HL

	CALL	CHKCHAR
	DEFB	"("

	LD	IX,GETBYT
	CALL	CALBAS0

	PUSH	AF			; register number

	CALL	CHKCHAR
	DEFB	","

	; capture variable to receive value
	; must be an int type
	LD	A, 0
	LD	($F6A5), A
	LD	IX, $5EA4
	CALL	CALBAS0

	// DE points to actual value
	// DE -2 is 2 byte NAME
	// DE - 3 is type

	PUSH	DE			; address to store 16 bit int of current register value

	CALL	CHKCHAR
	DEFB	")"

	EXX

	POP	HL
	DEC	HL
	DEC	HL
	DEC	HL
	LD	A, (HL)
	CP	2			; Check if it's a int
	JR	NZ, TYPE_MISMATCH
	INC	HL
	INC	HL
	INC	HL

	POP	AF

	DI
	OUT	($99), A		; set R#15 to A
	LD	A, 0x80 | 15
	OUT	($99), A		; retrieve its value
	IN	A, ($99)
	LD	D, A			; store in D

	XOR	A			; restore R#15 to 0
	OUT	($99), A		; set R#15 to 0
	LD	A, 0x80 | 15
	OUT	($99), A
	EI

	LD	(HL), D

	EXX

	AND	 A			; Clear carry flag
	RET

no_statements:
; If we get here, the strings don't match
	SCF				; Set carry flag
	POP	 HL		  	; Restore HL
	RET

SYNTAX_ERROR:
	LD	E, 2
	JR	BASIC_ERROR

TYPE_MISMATCH:
	LD	E, 13
	JR	BASIC_ERROR

MISSING_OPERAND:
	LD	E, 24
BASIC_ERROR:
	LD	IX, BASIC_ERR		; CALL the Basic error ha$dler
	JP	CALBAS0

CHKCHAR:
	CALL	GETPREVCHAR		; Get previous basic char
	EX	(SP), HL
	CP	(HL)			; Check if good char
	JR	NZ, SYNTAX_ERROR	; No, Syntax error
	INC	HL
	EX	(SP), HL
	INC	HL			; Get next basic char

GETPREVCHAR:
	DEC	HL
	LD	IX, CHRGTR
	JP	CALBAS0

CALBAS0:
	EXX
	LD	HL,CALBAS
	LD	($F1D0), HL
	EXX
	JP	CALLB0

MY_STATEMENTS:
	DEFW	SUPER_COLOR
	JP	SUPER_COLOR_FN

	DEFW	VDP_SET_REG
	JP	VDP_SET_REG_FN

	DEFW	VDP_GET_REG
	JP	VDP_GET_REG_FN

	DEFW	VDP_GET_STATUS
	JP	VDP_GET_STATUS_FN
	DEFB	0


SUPER_COLOR:
	DEFM	"SUPER_COLOR", 0

VDP_SET_REG:
	DEFM	"VDP_SET_REG", 0

VDP_GET_REG:
	DEFM	"VDP_GET_REG", 0

VDP_GET_STATUS:
	DEFM	"VDP_GET_STATUS", 0
