	include	"msx.inc"

	SECTION	CODE
;
; extern uint16_t extbio_rc2014_hello(void);
;
	PUBLIC	_extbio_rc2014_hello

_extbio_rc2014_hello:
	PUSH	IX
	LD	D, EXTBIO_RC2014		; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_HELLO_FN	; FUNCTION CODE
	LD	HL, 0
	CALL	EXTBIO				; RETURN VERSION IN L
	EI
	POP	IX
	RET
