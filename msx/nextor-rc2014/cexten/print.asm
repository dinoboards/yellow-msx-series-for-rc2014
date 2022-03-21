
	include	"asm.inc"
;-----------------------------------------------------------------------------
;
; Print a zero-terminated string on screen
; Input: HL = String address
; Protects AF
	PUBLIC	_print_string

_print_string:
	PUSH	AF
_PRINT_MORE:
	LD	A, (HL)
	AND	A
	JR	Z, _PRINT_DONE
	CALL	CHPUT
	INC	HL
	JR	_PRINT_MORE
_PRINT_DONE:
	POP	AF
	RET

	PUBLIC	_x_print_string
_x_print_string:
	PUSH	AF
_X_PRINT_MORE:
	LD	A, (HL)
	AND	A
	JR	Z, _X_PRINT_DONE
	CALL	PRINT_CHAR_VDP
	INC	HL
	JR	_X_PRINT_MORE
_X_PRINT_DONE:
	POP	AF
	RET

	PUBLIC	_print_char_vdp
_print_char_vdp:
	ld	a, l
PRINT_CHAR_VDP:
	; print to VDP (auto increments)
	out	($98), a
	ret

	PUBLIC	_print_init_screen0
_print_init_screen0:
    ; set pointer to 77,0 for screen 0
	xor	a
	ld	hl,40
	call	SetVdp_Write
	ret

PRINT_INIT_SCREEN1:
    ; set pointer to 0,0 for screen 1
    xor a
    ld hl,$1800
    call SetVdp_Write
    ret
;
; Set VDP address counter to write from address AHL (17-bit)
; Enables the interrupts
;
SetVdp_Write:
    rlc h
    rla
    rlc h
    rla
    srl h
    srl h
    di
    out ($99),a
    ld a,14 + 128
    out ($99),a
    ld a,l
    nop
    out ($99),a
    ld a,h
    or 64
    ei
    out ($99),a
    ret
