; include	"msx.inc"

; ; BASIC expanded statement ("CALL") handler.
; ; Works the expected way, except that if invoking CALBAS0 is needed,
; ; it must be done via the CALLB0 routine in kernel page 0.

PROCNM          EQU     $FD89
FRMEVL          EQU     #4C64
VALTYP          EQU     $F663
DAC             EQU     $F7F6
CALBAS	        EQU	$0159
GETBYT          EQU     $521C
BASIC_ERR       EQU     $406F
CHRGTR  EQU     $4666

DRV_BASSTAT:
; Save HL
        push    hl

	; HL points to PROCNM
	ld	hl, PROCNM

	; DE points to MY_STATEMENTS
	ld 	de, MY_STATEMENTS

; Compare strings
compare_loop:
	ld 	a, (de)
	or 	a
	JP 	z, no_statements  ; End of table, return

	push 	hl
	push 	de

	; Point DE to the string in the table
	ld 	a, (de)
	ld 	b, a
	inc 	de
	ld 	a, (de)
	ld 	d, a
	ld	e, b

; Compare strings
compare_strings:
	ld	a, (de)
	cp	(hl)
	jr	nz, no_match
	inc	de
	inc	hl
	ld	a, (de)
	or	a
	jr	nz, compare_strings

; If we get here, the strings match
	pop 	de
	pop 	hl

	inc 	de
	inc 	de
	ex	de, hl
	jp 	(hl)  ; Jump to the function

no_match:
    ; If we get here, the strings don't match
	pop 	de
	pop 	hl
	inc 	de
	inc 	de
	inc 	de
	inc 	de
	inc 	de
	jr 	compare_loop

// _SUPER_SCREEN(1) // super colour resolution
SUPER_SCREEN_FN:
        POP     HL

	CALL	CHKCHAR
	DEFB	"("

        LD	IX,GETBYT
        call    CALBAS0

	CP	1			; only super color res supported at this stage
	JP	NZ, SYNTAX_ERROR

	CALL	CHKCHAR
	DEFB	")"

	DI				; write 1 to R#31
	LD	A, 1
	OUT	($99), A
	LD	A, $80 + 31
	OUT	($99), A
	EI

        AND     A               	; Clear carry flag
        RET

SUPER_COLOR_FN:
	POP	HL

	CALL	CHKCHAR
	DEFB	"("

        LD	IX,GETBYT
        call    CALBAS0

	PUSH	AF			; save red

	CALL	CHKCHAR
	DEFB	","

        LD	IX,GETBYT
        call    CALBAS0

	PUSH	AF		        ; save greem

	CALL	CHKCHAR
	DEFB	","

        LD	IX,GETBYT
        call    CALBAS0

	PUSH	AF			; save blue

	CALL	CHKCHAR
	DEFB	")"

	EXX				; protect HL

	POP	DE			; DE = blue
	POP	BC			; BC = green
	POP	HL			; HL = red

	DI				; set bit 7 of R#31 to start RGB colour register loading
	LD	A, $80 | 1		; todo - do not assume colour res (bit 0) is set
	OUT	($99), A
	LD	A, $80 | 31
	OUT	($99), A

	LD	A, H
	OUT	($99), A		; load in Red
	LD	A, $80 | 30
	OUT	($99), A		; load into R#30

	LD	A, B
	OUT	($99), A		; load in Green
	LD	A, $80 | 30
	OUT	($99), A		; load into R#30

	LD	A, D
	OUT	($99), A		; load in Blue
	LD	A, $80 | 30
	OUT	($99), A		; load into R#30

	EI

	EXX				; restore HL
	AND	A               	; Clear carry flag
        RET

VDP_SET_REG_FN:
        POP     HL

	CALL	CHKCHAR
	DEFB	"("

        LD	IX,GETBYT
        call    CALBAS0

	PUSH	AF
        ; register number

	CALL	CHKCHAR
	DEFB	","

        LD	IX,GETBYT
        call    CALBAS0
	; register_value

	push	af
	CALL	CHKCHAR
	DEFB	")"

	POP	AF
	POP	DE	; /D = register_number; A = register_value

	DI
	OUT	($99), A
	LD	A, D
	OR	$80
	OUT	($99), A
	EI

        and     a               ; Clear carry flag
        ret

VDP_GET_REG_FN:
        pop     hl

	CALL	CHKCHAR
	DEFB	"("

        LD	IX,GETBYT
        call    CALBAS0

	push	af	; register number

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

	push 	de

	CALL	CHKCHAR
	DEFB	")"

	EXX

	POP	HL
	DEC	HL
	DEC	HL
	DEC	HL
	LD	A, (HL)
	CP	2 			; Check if it's a int
	JR	NZ, TYPE_MISMATCH
	INC	HL
	INC	HL
	INC	HL

	// store register value at HL

	// set reg 17 to A
	POP	AF

	LD	C, $99
	DI
	OUT	(C), A
	LD	A, 0x80 | 17
	OUT	(C), A
	IN	A, ($9B)
	EI

	LD	(HL), a

	EXX

        and     a               ; Clear carry flag
        ret

VDP_GET_STATUS_FN:
        pop     hl

	CALL	CHKCHAR
	DEFB	"("

        LD	IX,GETBYT
        call    CALBAS0

	push	af	; register number

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

	push 	de

	CALL	CHKCHAR
	DEFB	")"

	EXX

	POP	HL
        DEC	HL
	DEC	HL
	DEC	HL
	LD	A, (HL)
	CP	2 			; Check if it's a int
	JR	NZ, TYPE_MISMATCH
	INC	HL
	INC	HL
	INC	HL

	// store register value at HL

	// set reg 15 to A
	POP	AF

	LD	C, $99
	DI
	OUT	(C), A
	LD	A, 0x80 | 15
	OUT	(C), A
	IN	A, (C)
	LD	D, A

	XOR	A
	OUT	(C), A
	LD	A, 0x80 | 15
	OUT	(C), A

	EI

	LD	(HL), D


	EXX

        and     a               ; Clear carry flag
        ret

no_statements:
; If we get here, the strings don't match
        scf                     ; Set carry flag
        pop     hl              ; Restore HL
        ret

SYNTAX_ERROR:
        LD      E,2
	LD	IX,BASIC_ERR    ; Call the Basic error ha$dler
        JP      CALBAS0

TYPE_MISMATCH:
        LD      E,13
	LD	IX,BASIC_ERR    ; Call the Basic error ha$dler
        JP      CALBAS0

MISSING_OPERAND:
        LD      E,24
	LD	IX,BASIC_ERR    ; Call the Basic error ha$dler
        JP      CALBAS0

CHKCHAR:
	CALL	GETPREVCHAR	; Get previous basic char
	EX	(SP),HL
	CP	(HL) 	        ; Check if good char
	JR	NZ,SYNTAX_ERROR	; No, Syntax error
	INC	HL
	EX	(SP),HL
	INC	HL		; Get next basic char

GETPREVCHAR:
	DEC	HL
	LD	IX,CHRGTR
	JP      CALBAS0

CALBAS0:
        exx
        ld      hl,CALBAS
        ld      ($f1d0),hl
        exx
        JP    	CALLB0

MY_STATEMENTS:
	DEFW 	SUPER_SCREEN
	JP	SUPER_SCREEN_FN

	DEFW 	SUPER_COLOR
	JP	SUPER_COLOR_FN

	DEFW	VDP_SET_REG
	JP	VDP_SET_REG_FN

	DEFW	VDP_GET_REG
	JP	VDP_GET_REG_FN

	DEFW	VDP_GET_STATUS
	JP	VDP_GET_STATUS_FN
	DEFB	0


SUPER_SCREEN:
	DEFM	"SUPER_SCREEN", 0

SUPER_COLOR:
	DEFM	"SUPER_COLOR", 0

VDP_SET_REG:
        DEFM    "VDP_SET_REG", 0

VDP_GET_REG:
        DEFM    "VDP_GET_REG", 0

VDP_GET_STATUS:
	DEFM    "VDP_GET_STATUS", 0

; CALL VDP_SET_REG(REG_NUM, VALUE)
; CALL VDP_GET_REG(REG_NUM, <INT_VAR>)
; CALL VDP_GET_STATUS(REG_NUM, <INT_VAR>)

; _SUPER_SCREEN(1) // super colour resolution
; _SUPER_COLOR(R,G,B) // set super colour
; _SUPER_CLS
