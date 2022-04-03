
	include	"msx.inc"
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

	PUBLIC	_print_hex
; extern void print_hex(const char c) __z88dk_fastcall;

_print_hex:
    call __NUMTOHEX
    ld a, d
    call CHPUT
    ld a, e
    JP CHPUT

;       Subroutine      Convert 8-bit hexidecimal number to ASCII reprentation
;       Inputs          L - number to be printed
;       Outputs         DE - two byte ASCII values - D=65 / 'A' and E=66 / 'B'
__NUMTOHEX:
	ld	c, l   ; a = number to convert
    call _NTH1
    ld d, a
    ld a, c
    call _NTH2
    ld e, a
    ret  ; return with hex number in de
_NTH1:
    rra
    rra
    rra
    rra
_NTH2:
    or 0F0h
    daa
    add a, 0A0h
    adc a, 040h ; Ascii hex at this point (0 to F)
    ret