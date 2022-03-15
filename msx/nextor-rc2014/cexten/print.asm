
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
